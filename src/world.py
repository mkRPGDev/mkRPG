# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from map import Map
from character import Character

import const

class World():

    def __init__(self, screen_size):
        self.screen_size = screen_size
        self.current_map = Map(self.screen_size, "bg0.bg")
        
        self.scale = 1
        
        self.main_char = "Helyder"        
        self.characters = { "Helyder" : Character("Helyder", start_pos=(0,0)),
                            "Gu1ness" : Character("Gu1ness", start_pos=(3,6))
                          }
    
    def update(self):
        self.current_map.update()
        for character in self.characters.values():
            character.update()
    
    def render(self):
        res = pygame.Surface(self.screen_size)
        res.convert_alpha()
        image, rect_list = self.current_map.render()
        res.blit(image, self.current_map.pos_offset)
        for character in self.characters.values():
            char_offset = (self.current_map.pos_offset[0]+character.pos_offset[0],
                           self.current_map.pos_offset[1]+character.pos_offset[1])
            res.blit(character.render(), char_offset)
            rect_list.append(character.image.get_rect())
        return res, rect_list
    
    def move(self, dx, dy):
        newx = self.current_map.pos_offset[0]+dx
        newy = self.current_map.pos_offset[1]+dy
        movx = self.current_map.pos_offset[0]
        movy = self.current_map.pos_offset[1]
        if newx >= self.screen_size[0]-self.current_map.size[0]-\
           const.MOV_OFFSET and newx <= const.MOV_OFFSET:
            movx = newx
        else:
            if dx > 0 :
                movx = const.MOV_OFFSET
            else:
                movx = self.screen_size[0]-self.current_map.size[0]-\
                       const.MOV_OFFSET
        if newy >= self.screen_size[1]-self.current_map.size[1]-\
           const.MOV_OFFSET and newy <= const.MOV_OFFSET:
            movy = newy
        else:
            if dy > 0 :
                movy = const.MOV_OFFSET
            else:
                movy = self.screen_size[1]-self.current_map.size[1]-\
                       const.MOV_OFFSET
        
        self.current_map.move_to(movx, movy)
        
    
    def zoom(self, dz):
        if dz != 0 and\
            (self.current_map.size[0] > self.screen_size[0] and\
            self.current_map.size[1] > self.screen_size[1] and dz < 0) or \
            (self.scale < 1.3 and dz > 0):
            self.scale += dz
            self.current_map.zoom(dz)
            for character in self.characters.values():
                character.zoom(dz)
        
    def move_char(self, char_name, end_pos):
        start_pos = self.characters[char_name].current_cell
        path = self.current_map.compute_path(start_pos, end_pos)
        self.characters[char_name].set_path(path)
    
    def propagate_trigger(self, event):
        message = self.current_map.propagate_trigger(event)
        if message != "":
            return self.main_char+" "+message
        return ""