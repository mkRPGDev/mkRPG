import pygame
from pygame.locals import QUIT, K_ESCAPE, KEYUP, MOUSEBUTTONDOWN, MOUSEBUTTONUP
from pygame.locals import Rect

from sys import argv
from time import time
import configparser

from interface.const import *
from interface.interactions import registerInteractions, InteractionType
from shared.orders import OrderDispatcher
from interface.cache import ChunkCache

if USETCP:
    from shared.network import NetworkClient
else:
    from shared.networkudp import NetworkClient

#TODO trouver mieux cf world
with open("isserver.py","w") as file:
    file.write("SERVER = False\n")

import shared.world as world
from interface.printWorld import WorldViewer
from interface.interface import Interface
from interface.utils import add_to_rect_list

class Client(Interface):
    def __init__(self, w, p):
        super().__init__(w, p)

        pygame.display.init()
        self.screen_size = SCREEN_WIDTH, SCREEN_HEIGHT
        self.screen = pygame.display.set_mode(self.screen_size)
        self.interface = WorldViewer(self.world)
        self.perso = None

        print("Client initialised")

    def __del__(self):
        self.interface.end()
        print("Client killed")

    def getEvent(self):
        events = self.interface.get_event()
        keys = []
        for event in events:
            if event.type == QUIT:
                keys.append(K_ESCAPE)
            elif event.type == KEYUP:
                keys.append(event.key)
        return keys

    def set_perso(self, perso):
        self.perso = perso
        self.interface.set_perso(self.perso)

    def init(self):
        self.get_conf_file("client_conf.ini")

        pygame.font.init()
        self.font = pygame.font.Font(None, 18)

        self.background = pygame.Surface(self.screen_size)
        self.background = self.background.convert()
        self.background.fill((0, 0, 0))

        self.clock = pygame.time.Clock()

        self.mov_speed_x = 0
        self.mov_speed_y = 0

        self.refresh_counter = self.frame_counter(60)

        print("Ready to begin")

    def update(self):
        deltat = self.clock.tick(MAXFPS)/1000

        self.mov_speed_x, self.mov_speed_y, rect_list = self.update_view(pygame.mouse.get_pos(),
                                                    self.mov_speed_x,
                                                    self.mov_speed_y,
                                                    deltat)

        text = self.font.render("FPS : %d" % self.clock.get_fps(), 1, (255,0,0))

        new_rect_list = self.interface.update()

        if rect_list is None:
            rect_list = new_rect_list

        rect_list = add_to_rect_list(rect_list, text.get_rect().move(10,10))

        image = self.interface.render()

        self.screen.blit(self.background, (0,0))
        self.screen.blit(image, (0,0))
        self.screen.blit(text, (10,10))

        if next(self.refresh_counter) :
            pygame.display.flip()
        else : pygame.display.update(rect_list)

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
