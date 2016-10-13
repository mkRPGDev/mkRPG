# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from utils import load_png
from backgroundLayer import BackgroundLayer
from gridLayer import GridLayer
from cell import Cell

import const

class Map(pygame.Surface):
    
    def __init__(self, size, path):
        self.layers = [BackgroundLayer(path)]
        
        self.size = (self.layers[0].width, self.layers[0].height)
        c_width, c_height = Cell.cell_size(const.GRID_SCALE)
        g_width = int(self.size[0]/c_width)
        g_height = int(self.size[1]/c_height*2)
        
        self.layers.append(GridLayer(g_width, g_height ,const.GRID_SCALE))
        
        pygame.Surface.__init__(self, self.size)
        
        self.scale = 1
        self.pos_offset = (0,0)
    
    def zoom(self, dz):
        if (self.scale > 0.2 and dz < 1) or \
           (self.scale < 4 and dz > 1):
            for layer in self.layers:
                self.scale *= dz
                layer.zoom(dz)
    
    def move(self, dx, dy):
        self.pos_offset = (self.pos_offset[0]+dx, self.pos_offset[1]+dy)
    
    def render(self):
        self.fill((0,0,0))
        for layer in self.layers:
            layer.render()
            layer.draw(self)
        
        return self
        
    def update(self):
        for layer in self.layers:
            layer.update()