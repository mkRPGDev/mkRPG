# -*- coding: utf-8 -*-

import enum

import pygame
from pygame.locals import *

from utils import load_png
import const

class GlobalCache():
    
    def __init__(self):
        pass

    @classmethod
    def set(cls, key, value):
        cls.cache[key] = value
    
    @classmethod
    def get(cls, key):
        return cls.cache[key]
    
    @classmethod
    def clear(cls):
        cls.cache = {}
    
    @classmethod
    def keys(cls):
        return cls.cache.keys()
    
    @classmethod
    def show(cls):
        return str(cls.cache)

class ScaledCache(GlobalCache):
    
    @classmethod
    def add_scaled(cls, elt, scale):
        pass

    @classmethod
    def remove(cls, elt, scale):
        del cls.cache[elt][scale]
    
    @classmethod
    def get_elt(cls, elt, scale=1):
        if not elt in cls.keys():
            cls.init_elts([elt])
        cls.add_scaled(elt, scale)
        return cls.get(elt)[scale]
    
    @classmethod
    def free_cache(cls):
        for elt in cls.keys():
            if len(cls.get(elt).keys()) > const.MAX_CACHE_SIZE:
                cls.remove(elt, max(cls.get(elt).keys()))

class ImageCache(ScaledCache):
    
    cache = {}

    @classmethod
    def init_image_from_file(cls, image):
        cls.set(image, {1 : load_png(const.IMG_PATH+image)})
    
    @classmethod
    def init_image_from_surface(cls, key, image):
        cls.set(key, {1 : image})
    
    @classmethod
    def get_image(cls, image, scale=1):
        return cls.get_elt(image, scale)
    
    @classmethod
    def init_elts(cls, elts):
        cls.init_images(elts)
    
    @classmethod
    def init_images(cls, images):
        for image in images:
            if type(image) == str:
                cls.init_image_from_file(image)
                key = image
            else:
                cls.init_image_from_surface(image[0], image[1])
                key = image[0]
            
            scale = const.MIN_ZOOM
            while scale < const.MAX_ZOOM:
                cls.get_elt(image, scale)
                scale += const.ZOOM_STEP
    
    @classmethod
    def add_scaled(cls, image, scale):
        if scale not in cls.get(image).keys():
            cls.cache[image][scale] = \
                pygame.transform.scale(cls.cache[image][1],
                    (int(cls.cache[image][1].get_size()[0]*scale),
                     int(cls.cache[image][1].get_size()[1]*scale)))
        
        cls.free_cache()

from chunk import Chunk

class ChunkCache(ScaledCache):
    
    cache = {}
    
    @classmethod
    def init_chunk(cls, chunk):
        c = Chunk(*chunk)
        cls.set(chunk[0], {1 : c})
        return c.get_state()
    
    @classmethod
    def init_elts(cls, elts):
        cls.init_chunks(elts)
    
    @classmethod
    def init_chunks(cls, chunks):
        indexes_x = [chunk[0][0] for chunk in chunks]
        indexes_y = [chunk[0][1] for chunk in chunks]
        states = [[None\
                   for j in range(max(indexes_y)+1)]\
                   for i in range(max(indexes_x)+1)]
        for chunk in chunks:
            states[chunk[0][0]][chunk[0][1]] = cls.init_chunk(chunk)
            
            scale = const.MIN_ZOOM
            while scale < const.MAX_ZOOM:
                cls.get_elt(chunk[0], scale)
                scale += const.ZOOM_STEP
        return states
        
    
    @classmethod
    def get_chunk(cls, chunk_index, scale=1):
        return cls.get_elt(chunk_index, scale)
    
    @classmethod
    def add_scaled(cls, chunk_index, scale):
        if scale not in cls.get(chunk_index).keys():
            cls.cache[chunk_index][scale] =\
                cls.cache[chunk_index][1].scale_chunk(scale)
        
        cls.free_cache()