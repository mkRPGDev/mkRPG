# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

import const

def load_png(name, scale=1):
        """ Load image and return image object"""
        image = pygame.image.load(name)
        if image.get_alpha is None:
                image = image.convert()
        else:
                image = image.convert_alpha()
        width, height = image.get_size()
        if scale != 1:
            image = pygame.transform.scale(image, (int(width*scale), int(height*scale)))
        return image

def get_image(cache, key, scale=1):
    if not key in cache.keys():
        cache[key] = load_png(const.IMG_PATH+key, 1)
    return pygame.transform.scale(cache[key],
                                  (int(cache[key].get_size()[0]*scale),
                                   int(cache[key].get_size()[1]*scale)))
