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
        
        self.update_size()
        g_width = self.layers[0].g_width
        g_height = self.layers[0].g_height
        self.layers.append(GridLayer(g_width, g_height, const.GRID_SCALE))
        self.screen_size = size
        self.original_size = self.size
        
        pygame.Surface.__init__(self, self.size)
        
        self.scale = 1
        self.pos_offset = (0,0)
    
    def update_size(self):
        self.size = (self.layers[0].width, self.layers[0].height)
        pygame.Surface.__init__(self, self.size)
    
    def zoom(self, dz):
        if (self.size[0] > self.screen_size[0] and\
            self.size[1] > self.screen_size[1] and dz < 1) or \
           (self.scale < 1.3 and dz > 1):
            for layer in self.layers:
                self.scale *= dz
                layer.zoom(dz)
                self.update_size()
    
    def move(self, dx, dy):
        newx = self.pos_offset[0]+dx
        newy = self.pos_offset[1]+dy
        if newx >= self.screen_size[0]-self.size[0]-const.MOV_OFFSET and newx <= const.MOV_OFFSET:
            self.pos_offset = (newx, self.pos_offset[1])
        else:
            if dx > 0 :
                self.pos_offset = (const.MOV_OFFSET, self.pos_offset[1])
            else:
                self.pos_offset = (self.screen_size[0]-self.size[0]-const.MOV_OFFSET, self.pos_offset[1])
        if newy >= self.screen_size[1]-self.size[1]-const.MOV_OFFSET and newy <= const.MOV_OFFSET:                            
            self.pos_offset = (self.pos_offset[0], newy)
        else:
            if dy > 0 :
                self.pos_offset = (self.pos_offset[0], const.MOV_OFFSET)
            else:
                self.pos_offset = (self.pos_offset[0], self.screen_size[1]-self.size[1]-const.MOV_OFFSET)
        
        for layer in self.layers:
            layer.map_pos_offset = self.pos_offset
    
    def render(self):
        self.fill((0,0,0))
        for layer in self.layers:
            layer.render()
            layer.draw(self)
        
        return self
        
    def update(self, **kwargs):
        # Background layer
        self.layers[0].update()
        # Grid Layer
        self.layers[1].update()
    
    def propagate_trigger(self, event):
        self.layers[1].propagate_trigger(event)