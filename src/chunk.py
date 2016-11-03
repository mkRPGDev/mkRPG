# -*- coding: utf-8 -*-

import pygame
from pygame.locals import Rect

from backgroundLayer import BackgroundLayer

import const

class Chunk(pygame.sprite.Sprite):
    
    def __init__(self, index, cells, scale=1):
        pygame.sprite.Sprite.__init__(self)
        self.index = index
        self.cells = cells
        
        self.g_width = len(self.cells[0])
        self.g_height = len(self.cells)
        self.scale = scale
                
        self.init_chunk()
        
    def init_chunk(self):
        self.width = int((2*self.g_width+1)*const.CELL_WIDTH/2*self.scale)
        self.height = int((self.g_height+2)*const.CELL_HEIGHT/2*self.scale)
        self.pos = (self.index[1]*const.CHUNK_WIDTH*self.scale,
                    self.index[0]*const.CHUNK_HEIGHT*self.scale)
        self.rect = Rect(self.pos, (self.width, self.height))
        
        self.layers = [BackgroundLayer(self.cells, (self.width, self.height), self.scale)]
        self.image = self.layers[0].image
        self.rect = self.layers[0].rect.move(self.pos)
    
    def render(self):
        pass
    
    def scale_chunk(self, scale):
        return Chunk(self.index, self.cells, scale)
    
    def update(self, state, mouse_pos):
        self.set_state(state)
        new_state = None
        return new_state, []
    
    def click_trigger(self, mouse_pos):
        rel_m_pos = (mouse_pos[0]-self.pos[0],
                     mouse_pos[1]-self.pos[1])
        res = ''
        for layer in self.layers:
            message = layer.click_update(rel_m_pos)
            if message.split(" ")[0] == "MOVE":
                cell_x, cell_y = [int(i) for i in message.split(" ")[1].split(",")]
                cell = (self.index[0]*const.CHUNK_GRID_HEIGHT+cell_x,
                        self.index[1]*const.CHUNK_GRID_WIDTH+cell_y)
                if cell[0] >= 0 and cell[0] < const.CHUNK_GRID_WIDTH and\
                   cell[1] >= 0 and cell[1] < const.CHUNK_GRID_HEIGHT:
                    res += "MOVE "+str(cell[0])+","+str(cell[1])
            else:
                res += message     
        return res
        
    
    def set_state(self, state):
        pass
    
    def get_state(self):
        return None