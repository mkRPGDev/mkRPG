# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from utils import load_png, get_image
import const

class Cell(pygame.sprite.Sprite):
    
    image_cache = {}
    
    def __init__(self, index, scale=1):
        pygame.sprite.Sprite.__init__(self)
        self.scale = scale
        
        self.image = get_image(Cell.image_cache, const.IMG_PATH+"cell.png", scale)
        self.rect = self.image.get_rect()
        self.mask = pygame.mask.from_surface(get_image(Cell.image_cache, const.IMG_PATH+"mask.png", scale))
        self.index = index

        self.selected = False
    
    @staticmethod
    def cell_size(scale=1):
        rect = get_image(Cell.image_cache, const.IMG_PATH+"cell.png", scale).get_rect()
        return rect.width, rect.height
    
    def toggle_selected(self):
        if self.selected:
            self.image = get_image(Cell.image_cache, const.IMG_PATH+"cell.png", self.scale)
            self.selected = False
        else:
            self.image = get_image(Cell.image_cache, const.IMG_PATH+"cell_selected.png", self.scale)
            self.selected = True
    
    def mouseover_test(self, scale):
        pos = pygame.mouse.get_pos()
        pos = (pos[0], pos[1])
        if self.rect.collidepoint(pos):
            offset = (pos[0]-self.rect.x, pos[1]-self.rect.y)
            collision = self.mask.get_at(offset)
            if collision==1 and not self.selected:
                self.toggle_selected()
            elif collision==0 and self.selected:
                self.toggle_selected()
        else:
            if self.selected:
                self.toggle_selected()