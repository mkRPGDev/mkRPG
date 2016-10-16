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
    
    def update(self, **kwargs):
        self.mouseover_update(kwargs['pos_offset'])
    
    def cache_cells(self, img_set):
        points = [
                    (0, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, 0),
                    (const.CELL_WIDTH, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, const.CELL_HEIGHT),
                    (0,const.CELL_HEIGHT//2)
                 ]
        mask_points = [
                    (4, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, 2),
                    (const.CELL_WIDTH-4, const.CELL_HEIGHT//2),
                    (const.CELL_WIDTH//2, const.CELL_HEIGHT-2),
                    (4,const.CELL_HEIGHT//2)
                 ]
                 
        cell_image_default = pygame.Surface((const.CELL_WIDTH, const.CELL_HEIGHT), pygame.SRCALPHA)
        cell_image_default.convert_alpha()
        pygame.gfxdraw.aapolygon(cell_image_default, points, (40,40,40))
        cell_image_selected = pygame.Surface((const.CELL_WIDTH, const.CELL_HEIGHT), pygame.SRCALPHA)
        cell_image_selected.convert_alpha()
        pygame.gfxdraw.filled_polygon(cell_image_selected, points+mask_points, (200,200,200))
        cell_image_mask = pygame.Surface((const.CELL_WIDTH, const.CELL_HEIGHT), pygame.SRCALPHA)
        cell_image_mask.convert_alpha()
        pygame.gfxdraw.filled_polygon(cell_image_mask, mask_points, (255,255,255))
        
        GridLayer.image_cache[img_set['default']] = cell_image_default
        GridLayer.image_cache[img_set['selected']] = cell_image_selected
        GridLayer.image_cache[img_set['mask']] = cell_image_mask
    
    def render(self):
        pass
        
    def mouseover_update(self, pos_offset):
        for line in self.cells:
            for c in line:
                if c is not None:
                    c.mouseover_test(self.scale, pos_offset)
    
    def zoom(self, dz):
        self.scale *= dz
        self.__init__(self.g_width, self.g_height, self.scale)