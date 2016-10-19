# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from layer import Layer
import const

class GridLayer(Layer):

    image_cache = {}

    def __init__(self, width, height, scale=0.5):
        Layer.__init__(self, scale)
        
        self.g_width, self.g_height = width, height
        
        img_set = {'default':'cell', 
                   'mask':'mask', 
                   'selected':'cell_selected'}
        cells = [['default' for i in range(self.g_width)] for j in range(self.g_height)]
        
        self.cache_cells(img_set)
        self.make_grid(img_set, cells, GridLayer.image_cache)
    
    def update(self):
        self.mouseover_update()
    
    def cache_cells(self, img_set):
        points = [
                    (0, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, 0),
                    (const.CELL_WIDTH, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, const.CELL_HEIGHT),
                 ]
        mask_points = [
                    (4, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, 2),
                    (const.CELL_WIDTH-4, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, const.CELL_HEIGHT-2),
                 ]
                 
        cell_image_default = pygame.Surface((const.CELL_WIDTH, const.CELL_HEIGHT), pygame.SRCALPHA)
        cell_image_default.convert_alpha()
        pygame.draw.aalines(cell_image_default, (60,60,60), True, points)
        cell_image_selected = pygame.Surface((const.CELL_WIDTH, const.CELL_HEIGHT), pygame.SRCALPHA)
        cell_image_selected.convert_alpha()
        pygame.draw.lines(cell_image_selected, (200,200,200), True, points, 4)
        cell_image_mask = pygame.Surface((const.CELL_WIDTH, const.CELL_HEIGHT), pygame.SRCALPHA)
        cell_image_mask.convert_alpha()
        pygame.gfxdraw.filled_polygon(cell_image_mask, mask_points, (255,255,255))
        
        GridLayer.image_cache[img_set['default']] = cell_image_default
        GridLayer.image_cache[img_set['selected']] = cell_image_selected
        GridLayer.image_cache[img_set['mask']] = cell_image_mask
    
    def render(self):
        pass
        
    def mouseover_update(self):
        for line in self.cells:
            for c in line:
                if c is not None:
                    c.mouseover_test(self.scale, self.map_pos_offset)
    
    def zoom(self, dz):
        self.scale *= dz
        self.__init__(self.g_width, self.g_height, self.scale)
    
    def propagate_trigger(self, event):
        if event.type == MOUSEBUTTONUP and event.button == 1:
            for line in self.cells:
                for c in line:
                    c.click_test(self.scale, self.map_pos_offset)