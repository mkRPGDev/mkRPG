# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *
import pygame.gfxdraw

from utils import load_png, get_image

from triggers import MoveTrigger

import const

class Cell(pygame.sprite.Sprite):
    
    def __init__(self, index, img_set, default, image_cache, scale=1):
        pygame.sprite.Sprite.__init__(self)
        self.scale = scale
        self.img_set = img_set
        self.default = default
        self.image_cache = image_cache
        
        self.image = get_image(self.image_cache, img_set[default], scale)
        self.rect = self.image.get_rect()
        if 'mask' in img_set:
            self.mask = pygame.mask.from_surface(get_image(self.image_cache, img_set['mask'], scale))
        self.index = index

        self.selected = False
    
    def toggle_selected(self):
        if self.selected:
            self.image = get_image(self.image_cache, self.img_set[self.default], self.scale)
            self.selected = False
        else:
            self.image = get_image(self.image_cache, self.img_set['selected'], self.scale)
            self.selected = True
    
    def collision_test(self, scale, pos_offset):
        pos = pygame.mouse.get_pos()
        pos = (pos[0]-pos_offset[0], pos[1]-pos_offset[1])
        if self.rect.collidepoint(pos):
            offset = (pos[0]-self.rect.x, pos[1]-self.rect.y)
            collision = self.mask.get_at(offset)
            if collision==1:
                return True
        return False
    
    def mouseover_test(self, scale, pos_offset):
        collision = self.collision_test(scale, pos_offset)
        if collision and not self.selected:
            self.toggle_selected()
        elif not collision and self.selected:
            self.toggle_selected()
    
    def click_test(self, scale, pos_offset):
        collision = self.collision_test(scale, pos_offset)
        if collision:
            raise MoveTrigger(self.index)