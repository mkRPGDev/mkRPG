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
        self.net = NetworkClient(self.handleOrder)

        pygame.display.init()
        self.screen_size = SCREEN_WIDTH, SCREEN_HEIGHT
        self.screen = pygame.display.set_mode(self.screen_size)

        self.world = world.loadGame(path)
        self.interface = (WorldViewer if interface else Interface)(self.world)
        self.interactions = registerInteractions(path)
        self.perso = self.world.entities[0] # XXX bricolage
        self.orderDispatcher = OrderDispatcher(self.world, None)

        print("Client initialised")

    def __del__(self):
        self.net.kill()
        self.interface.end()
        print("Client killed")

    def run(self):
        self.net.start()
        self.get_conf_file("client_conf.ini")

        pygame.font.init()
        font = pygame.font.Font(None, 18)

        self.background = pygame.Surface(self.screen_size)
        self.background = self.background.convert()
        self.background.fill((0, 0, 0))

        clock = pygame.time.Clock()

        mov_speed_x = 0
        mov_speed_y = 0

        refresh_counter = self.frame_counter(60)

        print("Ready to begin")

        while True:
            deltat = clock.tick(60)/1000
            events = self.interface.get_event()
            for event in events:
                if event.type == QUIT or\
                   event.type == KEYUP and event.key == K_ESCAPE:
                    self.net.kill()
                    self.interface.end()
                    print("Exited properly")
                    return
                elif event.type == KEYUP:
                    for inte in self.interactions:
                        if (inte.type == InteractionType.Key and
                            inte.key == event.key):
                            self.net.sendEvent(self.__getattribute__(inte.target), inte.event)

            mov_speed_x, mov_speed_y, rect_list = self.update_view(pygame.mouse.get_pos(),
                                                        mov_speed_x,
                                                        mov_speed_y,
                                                        deltat)

            text = font.render("FPS : %d" % clock.get_fps(), 1, (255,0,0))

            new_rect_list = self.interface.update()

            if rect_list is None:
                rect_list = new_rect_list

            rect_list = add_to_rect_list(rect_list, text.get_rect().move(10,10))

            image = self.interface.render()

            self.screen.blit(self.background, (0,0))
            self.screen.blit(image, (0,0))
            self.screen.blit(text, (10,10))

            if next(refresh_counter) : pygame.display.flip()
            else : pygame.display.update(rect_list)
            self.interface.update()

    def frame_counter(self, n):
        i = n
        while 1:
            if i == n:
                i = 0
                yield True
            else:
                i += 1
                yield False

    def update_view(self, mouse_pos, mov_speed_x, mov_speed_y, deltat):
        posx,posy = mouse_pos

        if posx < MOV_OFFSET:
            if (mov_speed_x == 0 or mov_speed_x > 0) and mov_speed_x < self.get_conf("max_mov_speed", int):
                mov_speed_x += int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_x = 0
        elif posx > self.screen_size[0]-MOV_OFFSET:
            if (mov_speed_x == 0 or mov_speed_x < 0) and mov_speed_x > -self.get_conf("max_mov_speed", int):
                mov_speed_x -= int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_x = 0
        else:
            mov_speed_x = 0

        if posy > self.screen_size[1]-MOV_OFFSET:
            if (mov_speed_y == 0 or mov_speed_y < 0) and mov_speed_y < self.get_conf("max_mov_speed", int):
                mov_speed_y -= int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_y = 0
        elif posy < MOV_OFFSET:
            if (mov_speed_y == 0 or mov_speed_y > 0) and mov_speed_y > -self.get_conf("max_mov_speed", int):
                mov_speed_y += int(self.get_conf("mov_speed", int)*deltat)
            else:
                mov_speed_y = 0
        else:
                mov_speed_y = 0

        rect = self.interface.move(mov_speed_x, mov_speed_y)

        return mov_speed_x, mov_speed_y, rect

    def get_conf_file(self, path):
        self.conf = configparser.ConfigParser()
        self.conf.read(path)

    def get_conf(self, conf, type=str):
        return type(self.conf.get("CURRENT", conf))

    def init_cache(self):
        ImageCache.init_images(BG_IMGSET.values())
        ImageCache.init_images(GRID_IMGSET.values())

    def handleOrder(self, ident, order):
        emitter = world.BaseObject.ids[ident]
        self.orderDispatcher.treat(emitter, order)
        self.interface.update()

if __name__ == "__main__":
    cli = Client(argv[1] if len(argv)>1 else PATH).run()
