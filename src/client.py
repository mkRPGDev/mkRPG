import pygame
from pygame.locals import QUIT, K_ESCAPE, KEYUP, MOUSEBUTTONDOWN, MOUSEBUTTONUP
from pygame.locals import Rect

from sys import argv
from time import time
import configparser

from const import *
from interactions import registerInteractions, InteractionType
from orders import OrderDispatcher

if USETCP:
    from network import NetworkClient
else:
    from networkudp import NetworkClient

#TODO trouver mieux cf world
with open("isserver.py","w") as file:
    file.write("SERVER = False\n")

import world
from printWorld import WorldViewer, Interface
from utils import add_to_rect_list

#interface = argv[2] if len(argv)==3 else False
interface = True
# permet de désactiver pygame pour débugguer

class Client():
    def __init__(self, path):
        """ Initialize the client """
        self.net = NetworkClient(self.handleOrder) #Initialize network

        pygame.display.init() # Initialize pygame
        self.screen_size = SCREEN_WIDTH, SCREEN_HEIGHT
        self.screen = pygame.display.set_mode(self.screen_size)

        self.world = world.loadGame(path) # Initialize world

        self.init_cache() # Initialize cache

        # Pick interface (basic Interface is used for debugging)
        self.interface = (WorldViewer if interface else Interface)(self.world)

        # Initialize possible interactions
        self.interactions = registerInteractions(path)

        # Character we will be playing
        self.perso = self.world.entities[0] # XXX bricolage

        # Set order handler
        self.orderDispatcher = OrderDispatcher(self.world, None)

        print("Client initialised")

    def __del__(self):
        """ Kill network and interface """
        self.net.kill()
        self.interface.end()
        print("Client killed")

    def run(self):
        """ Start the game and loops until we exit it """

        # Start network
        self.net.start()

        # Load configuration file
        self.get_conf_file("client_conf.ini")

        pygame.font.init() # Initialize pygame.font for text rendering
        font = pygame.font.Font(None, 18)

        # Create black background to "erase" previous screen at each loop
        self.background = pygame.Surface(self.screen_size)
        self.background = self.background.convert()
        self.background.fill((0, 0, 0))

        # Initialize clock to cap FPS and get the time between two frames
        clock = pygame.time.Clock()

        # Initialize map scrolling speed
        mov_speed_x = 0
        mov_speed_y = 0

        # Initialize frame counter generator
        refresh_counter = self.frame_counter(MAXFPS)

        print("Ready to begin")

        # Main loop
        while True:
            # Cap FPS to MAXFPS and returns time between this call and the
            # last one in milliseconds
            deltat = clock.tick(MAXFPS)/1000

            # Handle events
            events = self.interface.get_event()
            for event in events:
                if event.type == QUIT or\
                   event.type == KEYUP and event.key == K_ESCAPE:
                    # Exit loop
                    del self
                    return
                elif event.type == KEYUP:
                    # Key was pressed, search for potential interaction
                    for inte in self.interactions:
                        if (inte.type == InteractionType.Key and
                            inte.key == event.key):
                            # Interaction was found, send appropriate event
                            # to server
                            self.net.sendEvent(
                                self.__getattribute__(inte.target),
                                inte.event)

            # Get new scrolling speed,
            # Rect list is the list of all rectangles on screen that have
            # been changed since previous loop, at this point its either None
            # or the whole screen depending on weither we've moved the map or
            # not
            mov_speed_x, mov_speed_y, rect_list = self.update_view(
                                                        pygame.mouse.get_pos(),
                                                        mov_speed_x,
                                                        mov_speed_y,
                                                        deltat)

            # Render current FPS, will be removed
            text = font.render("FPS : %d" % clock.get_fps(), 1, (255,0,0))

            # Get the list of all rectangles on screen that have been changed
            # by the update
            new_rect_list = self.interface.update()

            # Merge our rect lists
            if rect_list is None:
                rect_list = new_rect_list

            # Add our FPS text to the list (it changes at every loop)
            rect_list = add_to_rect_list(rect_list, text.get_rect().move(10,10))

            # Get the new image to put on screen
            image = self.interface.render()

            # Put everything on screen
            self.screen.blit(self.background, (0,0))
            self.screen.blit(image, (0,0))
            self.screen.blit(text, (10,10))

            # Most of the time we only update what has really changed,
            # sometimes we update the whole screen (once per second if the
            # game is running at MAXFPS) to correct small errors
            if next(refresh_counter) : pygame.display.flip()
            else : pygame.display.update(rect_list)

    def frame_counter(self, n):
        """ Generator that returns True every n calls """
        i = n
        while 1:
            if i == n:
                i = 0
                yield True
            else:
                i += 1
                yield False

    def update_view(self, mouse_pos, mov_speed_x, mov_speed_y, deltat):
        """ Returns new scrolling speed and the rectangle that have been changed
            and moves the map accordingly """
        posx,posy = mouse_pos

        # If our mouse x position is between 0 and MOV_OFFSET or between
        # screen width - MOV_OFFSET and screen_width, we increase
        # the scrolling speed in the right direction, else we set it to 0
        if posx < MOV_OFFSET:
            if (mov_speed_x == 0 or mov_speed_x > 0) and mov_speed_x <\
               self.get_conf("max_mov_speed", int):
                mov_speed_x += int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_x = 0
        elif posx > self.screen_size[0]-MOV_OFFSET:
            if (mov_speed_x == 0 or mov_speed_x < 0) and mov_speed_x >\
               -self.get_conf("max_mov_speed", int):
                mov_speed_x -= int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_x = 0
        else:
            mov_speed_x = 0

        # Same as before for y position
        if posy > self.screen_size[1]-MOV_OFFSET:
            if (mov_speed_y == 0 or mov_speed_y < 0) and mov_speed_y <\
               self.get_conf("max_mov_speed", int):
                mov_speed_y -= int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_y = 0
        elif posy < MOV_OFFSET:
            if (mov_speed_y == 0 or mov_speed_y > 0) and mov_speed_y >\
               -self.get_conf("max_mov_speed", int):
                mov_speed_y += int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_y = 0
        else:
                mov_speed_y = 0

        # We move the map and get the rectangle of modifications
        rect = self.interface.move(mov_speed_x, mov_speed_y)

        return mov_speed_x, mov_speed_y, rect

    def get_conf_file(self, path):
        """ Load path. Path should be a .ini file """
        self.conf = configparser.ConfigParser()
        self.conf.read(path)

    def get_conf(self, conf, type=str):
        """ Return value of parameter conf in loaded conf file """
        return type(self.conf.get("CURRENT", conf))

    def init_cache(self):
        """ Initialize cache with the images we will need """
        ImageCache.init_images(BG_IMGSET.values())
        ImageCache.init_images(GRID_IMGSET.values())

    def handleOrder(self, ident, order):
        """ Handle the given order on object ident """
        emitter = world.BaseObject.ids[ident]
        self.orderDispatcher.treat(emitter, order)
        self.interface.update()

if __name__ == "__main__":
    cli = Client(argv[1] if len(argv)>1 else PATH).run()
