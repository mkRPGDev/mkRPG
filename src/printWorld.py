# -*- coding: utf-8 -*-

import pygame
from pygame.locals import Rect

from time import sleep

import world
from map import MapViewer
from character import Character
from utils import merge_rect_lists
from cache import ImageCache
import const

class Interface: # XXX inutile en python
    def __init__(self, w):
        assert type(w) == world.World
        self.world = w
    def update(self): pass
    def end(self): pass
    def get_event(self):
        while True: sleep(1)

class WorldViewer(Interface):

    def __init__(self, w):
        super().__init__(w)
        self.screen_size = const.SCREEN_WIDTH, const.SCREEN_HEIGHT
        self.current_map = MapViewer(self.world.currentMap, self.world)

        self.main_char = "Helyder"
        self.characters = { "Helyder" : Character("Helyder", start_pos=(0,0),
                            skin=const.IMG_PATH+"character_skin.png"),
                            "Gu1ness" : Character("Gu1ness", start_pos=(3,6),
                            skin=const.IMG_PATH+"skeleton_skin.png")
                          }

    def get_event(self):
        return pygame.event.get()

    def end(self):
        pygame.display.quit()

    def update(self):
        rect_arr = self.current_map.update()
        charac_rects = []
        for character in self.characters.values():
            rects = character.update()
            if rects is not None:
                for i in range(len(rects)):
                    rects[i] = rects[i].move(self.current_map.pos_offset)
                charac_rects = merge_rect_lists(charac_rects, rects)

        return merge_rect_lists(charac_rects, rect_arr)

    def render(self):
        res = pygame.Surface(self.screen_size)
        res.convert_alpha()
        image = self.current_map.render()

        offsetx, offsety = self.current_map.chunk_pos

        if self.current_map.current_chunk[0] > 0:
            offsetx = const.CHUNK_WIDTH
        if self.current_map.current_chunk[1] > 0:
            offsety += const.CHUNK_WIDTH

        res.blit(image, (offsetx, offsety))
        for character in self.characters.values():
            char_offset = (self.current_map.pos_offset[0]+character.pos_offset[0],
                           self.current_map.pos_offset[1]+character.pos_offset[1])
            res.blit(character.render(), char_offset)
        return res

    def move(self, dx, dy):
        if dx != 0 or dy != 0:
            self.current_map.move(dx, dy)
            return [Rect((0,0),self.screen_size)]


    def zoom(self, dz):
        if dz != 0 and\
           ((self.scale > const.MIN_ZOOM and dz < 0) or \
            (self.scale < const.MAX_ZOOM and dz > 0)):
            self.current_map.zoom(dz)
            for character in self.characters.values():
                character.zoom(dz)

            return [Rect((0,0),self.screen_size)]

    def move_char(self, char_name, end_pos):
        start_pos = self.characters[char_name].current_cell
        path = self.current_map.compute_path(start_pos, end_pos)
        self.characters[char_name].set_path(path)

    def propagate_trigger(self, event):
        return self.current_map.propagate_trigger(event)
