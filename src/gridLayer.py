# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from cache import ImageCache

from layer import Layer
import const

class GridLayer(Layer):

    def __init__(self, width, height, scale=0.5):
        Layer.__init__(self, scale)
        
        self.g_width, self.g_height = width, height

        cells = [['default' for i in range(self.g_width)] for j in range(self.g_height)]

        self.make_grid(const.GRID_IMGSET, cells)
    
    def update(self):
        self.mouseover_update()
    
    def render(self):
        pass
        
    def mouseover_update(self):
        for line in self.cells:
            for c in line:
                if c is not None:
                    c.mouseover_test(self.scale, self.map_pos_offset)
    
    def zoom(self, dz):
        self.scale += dz
        self.__init__(self.g_width, self.g_height, self.scale)
    
    def propagate_trigger(self, event):
        res = ""
        if event.type == MOUSEBUTTONUP and event.button == 1:
            for line in self.cells:
                for c in line:
                    res += c.click_test(self.scale, self.map_pos_offset)
        return res
    
    def make_walkables(self):
        return [[c.walkable if c is not None else False for c in c_line]\
                 for c_line in self.cells]