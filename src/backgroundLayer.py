# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from cache import ImageCache

from layer import Layer
from utils import merge_rect_lists
import const

class BackgroundLayer(Layer):
    
    def __init__(self, cell_ids, size, scale=1):
        Layer.__init__(self, size, scale)
        
        self.cell_ids = cell_ids
        self.grid_cell_ids = self.get_grid_info(self.cell_ids)
        
        self.g_width = len(self.cell_ids[0])
        self.g_height = len(self.cell_ids)
        
        self.init_layer()
    
    def init_layer(self):
        self.image = self.make_grid(const.BG_IMGSET, self.cell_ids)
        self.image.blit(self.make_grid(const.GRID_IMGSET, self.grid_cell_ids), (0,0))
        self.rect = Rect((0,0), self.image.get_size())
        
        self.mouse_iso = ImageCache.get_image(const.GRID_IMGSET["mouse_iso"], self.scale)
        self.selected_cell = None
    
    def render(self):
        pass
    
    def collision_test(self, mouse_pos):
        cell_size = (const.CELL_WIDTH, const.CELL_HEIGHT)
        cell_pos = (mouse_pos[0]%cell_size[0], mouse_pos[1]%cell_size[1])

        cell_line = 2*(mouse_pos[1]//cell_size[1])
        cell_col = mouse_pos[0]//cell_size[0]
        
        if cell_line >= 0 and cell_line < self.g_height and\
           cell_col >= 0 and cell_col < self.g_width:
            
            pixel_col = self.mouse_iso.get_at(cell_pos)
            
            if pixel_col == (255,0,0,255):
                if cell_line % 2 == 0:
                    cell_col -= 1
                cell_line -= 1
            elif pixel_col == (0,255,0,255):
                if cell_line % 2 == 0:
                    cell_col -= 1
                cell_line += 1
            elif pixel_col == (0,0,255,255):
                if cell_line % 2 == 1:
                    cell_col += 1
                cell_line += 1
            elif pixel_col == (255,255,0,255):
                if cell_line % 2 == 1:
                    cell_col += 1
                cell_line -= 1
            elif pixel_col == (255,255,255,255):
                pass
            else:
                print("Couleur introuvable %s" % str(pixel_col))
                
        return cell_line, cell_col
    
    def click_update(self, mouse_pos):
        cell = self.collision_test(mouse_pos)
        if cell is not None:
            index = (cell[0],
                     cell[1])
            return "MOVE "+str(index[0])+","+str(index[1])
        return ""
    
    def get_grid_info(self, cell_ids):
        return [[const.CELL_MATCH[cell_ids[i][j]]\
               for j in range(len(cell_ids[0]))]\
               for i in range(len(cell_ids))]
    
    def zoom(self, dz):
        self.scale += dz
        self.update_grid()