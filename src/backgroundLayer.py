# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from layer import Layer
from utils import load_png, get_image

class BackgroundLayer(Layer):
    
    image_cache = {}
    
    def __init__(self, path, scale=1):
        Layer.__init__(self)
        
        self.scale = scale
        
        self.path = path
        
        self.image = pygame.sprite.Sprite()       
        self.bitmap = get_image(BackgroundLayer.image_cache, path, scale)
        self.image.image = self.bitmap
        self.width, self.height = self.image.image.get_size()
        self.image.rect = Rect((0,0), (self.width, self.height))
        self.image.image = self.bitmap
                
    
    def render(self):
        self.empty()
        self.add(self.image)
    
    def zoom(self, dz):
        self.scale *= dz
        self.image.image = get_image(BackgroundLayer.image_cache, self.path, self.scale)
        self.image.rect = Rect((0,0), self.image.image.get_size())