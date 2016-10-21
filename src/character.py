# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from cache import ImageCache
from utils import load_png

import const

class Character():
    
    def __init__(self, name, start_pos, skin):
        self.skin = {}
        
        self.name = name
        self.action = None
        self.scale = 1
        
        self.make_skin(skin)
        # 0 : up
        # 1 : left 
        # 2 : down
        # 3 : right
        self.orientation = 2
        self.image = ImageCache.get_image(self.skin['default'][2])
        self.current_image = self.skin['default'][2]
        
        self.game_frame_count = 0
        self.anim_frame_count = 0
        
        self.current_cell = start_pos
        self.path = []
        self.pos_offset = self.get_cell_pos_by_index(self.current_cell)
    
    def render(self):
        return self.image
    
    def update(self):
        self.move()
        
        self.update_skin()
    
    def update_skin(self):
        if self.action is not None:
            if self.action == "standby":
                self.anim_frame_count = 0
                self.game_frame_count = 0
                self.image = ImageCache.get_image(
                    self.skin['default'][self.orientation],
                    self.scale                   )
                self.current_image = self.skin['default'][self.orientation]
                self.action = None
            else:
                if self.game_frame_count >= const.CHAR_ANIM_SPEED:
                    if self.action == "moving":
                        key = 'walk'
                    self.game_frame_count = 0
                    self.anim_frame_count += 1
                    self.anim_frame_count %= len(self.skin[key][self.orientation])
                    self.image = ImageCache.get_image(
                        self.skin[key][self.orientation][self.anim_frame_count],
                        self.scale                   )
                    self.current_image = self.skin[key][self.orientation][self.anim_frame_count]
                self.game_frame_count += 1
            
    
    def zoom(self, dz):
        self.scale += dz
        self.image = ImageCache.get_image(self.current_image, self.scale)
        self.pos_offset = self.get_cell_pos_by_index(self.current_cell)
    
    def set_path(self, path):
        self.path = path
    
    def make_skin(self, skin):
        image_skin = load_png(skin)
        
        offsetx = 16
        offsety = 16
        char_width = 31
        char_height = 46
        spacex = 33
        spacey = 18
        
        self.skin['spell'] = {0:[],1:[],2:[],3:[]}
        self.skin['stab'] = {0:[],1:[],2:[],3:[]}
        self.skin['walk'] = {0:[],1:[],2:[],3:[]}
        self.skin['cut'] = {0:[],1:[],2:[],3:[]}
        self.skin['shoot'] = {0:[],1:[],2:[],3:[]}
        self.skin['default'] = {}
        self.skin['death'] = {}
        
        for line in range(20):
            if line // 4 ==  0:
                col_max = 7
                key = 'spell'
            if line // 4 ==  1:
                col_max = 8
                key = 'stab'
            if line // 4 ==  2:
                col_max = 9
                key = 'walk'
            if line // 4 ==  3:
                col_max = 6
                key = 'cut'
            if line // 4 ==  4:
                col_max = 7
                key = 'shoot'
            
            orientation = line % 4
            
            for col in range(col_max):
                posx = offsetx + col*(char_width+spacex)
                posy = offsety + line*(char_height+spacey)
                image = image_skin.subsurface(
                            Rect(posx,posy,char_width,char_height)
                                             ).copy()
                ImageCache.init_image_from_surface(self.name+"_"+key+"_"+\
                                                  str(orientation)+"_"+str(col), 
                                                   image)
                self.skin[key][orientation].append(self.name+"_"+key+"_"+\
                                                  str(orientation)+"_"+str(col))
                                                                                
        self.skin['default'][0] = self.skin['walk'][0][0]
        self.skin['default'][1] = self.skin['walk'][1][0]
        self.skin['default'][2] = self.skin['walk'][2][0]
        self.skin['default'][3] = self.skin['walk'][3][0]
        
        self.skin['death'][2] = []
        for col in range(6):
            posx = offsetx + col*(char_width+spacex)
            posy = offsety + 20*(char_height+spacey)
            image = image_skin.subsurface(
                        Rect(posx,posy,char_width,char_height)
                                         ).copy()
            self.skin[key][2].append(image)
        
        image_skin.fill((0,0,0))

    def get_cell_pos_by_index(self, index):
        c_h, c_w = index
        w, h = self.image.get_size()
        if c_h % 2 == 0:
            return (int(const.CELL_WIDTH*self.scale*(c_w+0.5)-w/2), 
                        int(const.CELL_HEIGHT*self.scale/2*(c_h+1)-h))
        else:
            return (int(const.CELL_WIDTH*self.scale/2+\
                            const.CELL_WIDTH*self.scale*(c_w+0.5)-w/2), 
                        int(const.CELL_HEIGHT*self.scale/2*(c_h+1)-h))
    
    def move(self):
        if len(self.path)>0:
            next_pos = self.get_cell_pos_by_index(self.path[0])
            self.action = "moving"
            
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
            
            movx = int(movx*self.scale)
            movy = int(movy*self.scale)
            
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
                if len(self.path) == 0:
                    self.action = "standby"