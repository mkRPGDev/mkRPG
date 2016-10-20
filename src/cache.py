# -*- coding: utf-8 -*-

import enum

import pygame
from pygame.locals import *

from utils import load_png
import const

class GlobalCache():
    
    cache = {}
    
    def __init__(self):
        pass

    @staticmethod
    def set(key, value):
        GlobalCache.cache[key] = value
    
    @staticmethod
    def get(key):
        return GlobalCache.cache[key]
    
    @staticmethod
    def clear():
        GlobalCache.cache = {}
    
    @staticmethod
    def keys():
        return GlobalCache.cache.keys()
    
    @staticmethod
    def show():
        return str(GlobalCache.cache)

class ImageCache(GlobalCache):
    
    cache = {}
    
    @staticmethod
    def init_image_from_file(image):
        ImageCache.set(image, {1 : load_png(const.IMG_PATH+image)})
    
    @staticmethod
    def init_image_from_surface(key, image):
        ImageCache.set(key, {1 : image})
    
    @staticmethod
    def init_images(images):
        for image in images:
            if type(image) == str:
                ImageCache.init_image_from_file(image)
            else:
                ImageCache.init_image_from_surface(image[0], image[1])
    
    @staticmethod
    def init_mask(mask):
        ImageCache.set("mask_"+mask, {1:\
            pygame.mask.from_surface(ImageCache.get_image(mask))})
    
    @staticmethod
    def add_scaled(image, scale):
        if scale not in ImageCache.get(image).keys():
            GlobalCache.cache[image][scale] = \
                pygame.transform.scale(GlobalCache.cache[image][1],
                    (int(GlobalCache.cache[image][1].get_size()[0]*scale),
                     int(GlobalCache.cache[image][1].get_size()[1]*scale)))
        
        ImageCache.free_cache()
    
    def add_scaled_mask(mask, scale):
        if scale not in ImageCache.get(mask).keys():
            GlobalCache.cache["mask_"+mask][scale] =\
                pygame.mask.from_surface(ImageCache.get_image(mask, scale))
    @staticmethod
    def remove(image, scale):
        del GlobalCache.cache[image][scale]
    
    @staticmethod
    def get_image(image, scale=1):
        if not image in ImageCache.keys():
            ImageCache.init_images([image])
        ImageCache.add_scaled(image, scale)
        return ImageCache.get(image)[scale]
    
    @staticmethod
    def get_mask(mask, scale=1):
        if not mask in ImageCache.keys():
            ImageCache.init_mask(mask)
        ImageCache.add_scaled_mask(mask, scale)
        return ImageCache.get("mask_"+mask)[scale]
    
    @staticmethod
    def free_cache():
        for image in ImageCache.keys():
            if len(ImageCache.get(image).keys()) > const.MAX_CACHE_SIZE:
                ImageCache.remove(image, max(ImageCache.get(image).keys()))