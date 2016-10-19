# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from map import Map
from character import Character

from triggers import MoveTrigger

import const

class World():

    def __init__(self, screen_size):
        self.screen_size = screen_size
        self.current_map = Map(self.screen_size, "bg0.bg")
        
        self.character = Character("Helyder")
    
    def update(self):
        self.current_map.update()
        self.character.update()
    
    def render(self):
        res = pygame.Surface(self.screen_size)
        res.convert_alpha()
        res.blit(self.current_map.render(), self.current_map.pos_offset)
        char_offset = (self.current_map.pos_offset[0]+self.character.pos_offset[0],
                       self.current_map.pos_offset[1]+ self.character.pos_offset[1])
        res.blit(self.character.render(), char_offset)
        return res
    
    def move(self, dx, dy):
        self.current_map.move(dx, dy)
    
    def zoom(self, dz):
        self.current_map.zoom(dz)
    
    def propagate_trigger(self, event):
        try:
            self.current_map.propagate_trigger(event)
        except MoveTrigger as m:
            self.character.compute_path(m.target)