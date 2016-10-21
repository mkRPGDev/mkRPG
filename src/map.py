# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from utils import load_png, WalkableGraph
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
        self.walkablesGraph = WalkableGraph(self.layers[1].make_walkables())
        
        pygame.Surface.__init__(self, self.size)
        
        self.scale = 1
        self.pos_offset = (0,0)
    
    def update_size(self):
        self.size = (self.layers[0].width, self.layers[0].height)
        pygame.Surface.__init__(self, self.size)
    
    def zoom(self, dz):
        self.scale += dz
        for layer in self.layers:
            layer.zoom(dz)
        self.update_size()
    
    def move_to(self, newx, newy):
        self.pos_offset = (newx, newy)
        
        for layer in self.layers:
            layer.map_pos_offset = self.pos_offset
    
    def render(self):
        self.fill((0,0,0))
        res = []
        for layer in self.layers:
            layer.render()
            res += layer.draw(self)
        
        return self, res
        
    def update(self, **kwargs):
        # Background layer
        self.layers[0].update()
        # Grid Layer
        self.layers[1].update()
    
    def propagate_trigger(self, event):
        return self.layers[1].propagate_trigger(event)
    
    def compute_path(self, start_pos, end_pos):
        return self.walkablesGraph.get_path(start_pos, end_pos)