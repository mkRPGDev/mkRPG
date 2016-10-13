# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from layer import Layer
from cell import Cell

class GridLayer(Layer):

    def __init__(self, width, height, scale=0.5):
        Layer.__init__(self)
        
        self.scale = scale
        self.g_width, self.g_height = width, height
        
        self.cells = []
        self.make_grid()
        
        self.screen_size = (int(self.g_width*self.c_width), int((self.g_height+self.g_height%2)*self.c_height/2))
        
    
    def make_grid(self):
        self.empty()
        self.c_width, self.c_height = Cell.cell_size(self.scale)
        
        line = 0
        col = 0
        cell_line = []
        nb_cells = int(self.g_height/2*self.g_width+self.g_height/2*(self.g_width-1))+self.g_height%2
        for i in range(nb_cells):
            cell_line.append(Cell((line,col+line%2),self.scale))
            self.add(cell_line[-1])
            if line%2 == 0:
                cell_line[-1].rect = cell_line[-1].rect.move((self.c_width*col, self.c_height//2*line))
            else:
                cell_line[-1].rect = cell_line[-1].rect.move((self.c_width//2+self.c_width*col, self.c_height//2*line))     
            col += 1
            if (line%2 == 0 and col >= self.g_width) or (line%2 == 1 and col >= self.g_width-1):
                line += 1
                col = 0
                if len(cell_line) < self.g_width:
                    cell_line = [None] + cell_line
                self.cells.append(cell_line)
                cell_line = []
    
    def update(self):
        self.mouseover_update()
    
    def render(self):
        pass
        
    
    def mouseover_update(self):
        for line in self.cells:
            for c in line:
                if c is not None:
                    c.mouseover_test(self.scale)
    
    def zoom(self, dz):
        self.scale *= dz
        self.__init__(self.g_width, self.g_height, self.scale)