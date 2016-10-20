# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

import struct

from cache import ImageCache

from layer import Layer
from utils import load_png
import const

class BackgroundLayer(Layer):
    
    image_cache = {}
    
    def __init__(self, path, scale=1):
        Layer.__init__(self, scale)
        
        self.path = path
        self.cell_ids = self.load_bg(const.BG_PATH+path)
        
        self.make_grid(const.BG_IMGSET, self.cell_ids)
        
        self.g_width = len(self.cell_ids[0])
        self.g_height = len(self.cell_ids)
        self.width = int(self.g_width*const.CELL_WIDTH*scale) 
        self.height = int((self.g_height+1)*const.CELL_HEIGHT/2*scale)
        
        # print("Grid size: %dx%d - %d cells" %
                # (self.g_width, self.g_height,
                 # self.g_width*(self.g_height//2+1)\
                 # +(self.g_width-1)*self.g_height//2))
    
    def load_bg(self, path):
        bg = open(path, 'rb')
        
        cells = []
        
        cont = True
        found0 = False
        cell_line = []
        while cont:
            next = struct.unpack('@B', bg.read(1))[0]
            if next == 0:
                if found0:
                    cont = False
                else:
                    found0 = True
                    cells.append(cell_line)
                    cell_line = []
            elif next == 255:
                cell_line.append(None)
                found0 = False
            else:
                cell_line.append(next)
                found0 = False
        
        return cells
    
    def zoom(self, dz):
        self.scale += dz
        self.__init__(self.path, self.scale)