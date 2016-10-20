# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

import const

class Character():
    
    def __init__(self, name, start_pos):
        self.skin = pygame.Surface((20,20), pygame.SRCALPHA)
        self.skin.convert_alpha()
        pygame.draw.circle(self.skin, (0,0,0), (10,10), 10)
        self.image = self.skin
        self.name = name
        self.scale = 1
        
        self.current_cell = start_pos
        self.path = []
        self.pos_offset = self.get_cell_pos_by_index(self.current_cell)
    
    def render(self):
        return self.image
    
    def update(self):
        self.move()
    
    def zoom(self, dz):
        self.scale += dz
        self.image = pygame.transform.scale(self.skin,
                                    (int(self.skin.get_size()[0]*self.scale),
                                    int(self.skin.get_size()[1]*self.scale)))
        self.pos_offset = self.get_cell_pos_by_index(self.current_cell)
    
    def set_path(self, path):
        self.path = path
    
    def get_cell_pos_by_index(self, index):
        c_h, c_w = index
        w, h = self.image.get_size()
        if c_h % 2 == 0:
            return (int(const.CELL_WIDTH*self.scale*c_w+\
                            const.CELL_WIDTH*self.scale/2-w/2), 
                        int(const.CELL_HEIGHT*self.scale/2*c_h+\
                            const.CELL_HEIGHT*self.scale/2-h/2))
        else:
            return (int(const.CELL_WIDTH*self.scale/2+\
                            const.CELL_WIDTH*self.scale*c_w+\
                            const.CELL_WIDTH*self.scale/2-w/2), 
                        int(const.CELL_HEIGHT*self.scale/2*c_h+\
                            const.CELL_HEIGHT*self.scale/2-h/2))
    
    def move(self):
        if len(self.path)>0:
            next_pos = get_cell_pos_by_index(self.path[0])
            
            distx = next_pos[0]-self.pos_offset[0]
            disty = next_pos[1]-self.pos_offset[1]
            movx = 0
            movy = 0
            
            if distx == 0:
                if disty > 0:
                    movy = const.CHAR_MOV_SPEED
                elif disty < 0:
                    movy = -const.CHAR_MOV_SPEED
            elif disty == 0:
                if distx > 0:
                    movx = const.CHAR_MOV_SPEED
                elif distx < 0:
                    movx = -const.CHAR_MOV_SPEED
            else:
                if distx > 0:
                    movx = const.CHAR_MOV_SPEED//2
                elif distx < 0:
                    movx = -const.CHAR_MOV_SPEED//2
                if disty > 0:
                    movy = const.CHAR_MOV_SPEED//2
                elif disty < 0:
                    movy = -const.CHAR_MOV_SPEED//2
            
            if movx >= distx and distx>0 or\
               movx <= distx and distx<0:
                movx = distx
            if movy >= disty and disty>0 or\
               movy <= disty and disty<0:
                movy = disty
            
            self.pos_offset = (self.pos_offset[0]+movx,
                               self.pos_offset[1]+movy)
            
            if self.pos_offset == next_pos:
                self.current_cell = self.path[0]
            
            if self.current_cell == self.path[0]:
                del self.path[0]