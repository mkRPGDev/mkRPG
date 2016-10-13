# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

class Layer(pygame.sprite.Group):
    
    def __init__(self, *sprites):
        pygame.sprite.Group.__init__(self, sprites)
    
    def render(self):
        pass
    
    def update(self):
        pass