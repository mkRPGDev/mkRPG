# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from multiprocessing import Pool

from cell import Cell
import const

class Layer(pygame.sprite.Group):
    
    def __init__(self, scale=1):
        pygame.sprite.Group.__init__(self)
        self.cells = []
        self.scale = scale
        self.width = 0
        self.height = 0
        self.g_width = 0
        self.g_height = 0
    
    def render(self):
        pass
    
    def update(self, **kwargs):
        pass
    
    def create_cell_line(self, c_line, line, img_set, image_cache):
            cell_line = []
            col = 0
            for cell in c_line:
                if cell is not None:
                    cell_line.append(Cell((line,col+line%2),img_set,cell,image_cache,self.scale))
                    if line%2 == 0:
                        cell_line[-1].rect = cell_line[-1].rect.move((int(const.CELL_WIDTH*self.scale*col), int(const.CELL_HEIGHT*self.scale/2*line)))
                    else:
                        cell_line[-1].rect = cell_line[-1].rect.move((int(const.CELL_WIDTH*self.scale/2+const.CELL_WIDTH*self.scale*col), int(const.CELL_HEIGHT*self.scale/2*line)))     
                    col += 1
            if len(cell_line) < len(cell_line):
                cell_line = [None] + cell_line
            return cell_line
    
    def make_grid(self, img_set, cell_ids, image_cache):
        self.empty()
        
        line = 0
        for c_line in cell_ids:
            cell_line = self.create_cell_line(c_line, line, img_set, image_cache)
            for c in cell_line:
                if c is not None:
                    self.add(c)
            self.cells.append(cell_line)
            line += 1
    
    def zoom(self, dz):
        pass