# -*- coding: utf-8 -*-

import pygame

from cache import ImageCache
import const
import utils
from math import pi

class Layer(pygame.sprite.Sprite):
    
    def __init__(self, size, scale=1):
        pygame.sprite.Sprite.__init__(self)
        self.cells = []
        self.scale = scale
        self.g_width = 0
        self.g_height = 0
        self.size = size
    
    def render(self, surf):
        return self.sprites
    
    def update(self):
        pass
        
    def get_cell_pos(self, c_line, c_col, size_image):
        #if c_line%2 == 0:
        #    pos = (int((const.CELL_WIDTH*c_col-size_image[0]+const.CELL_WIDTH)*self.scale), 
        #           int((const.CELL_HEIGHT/2*c_line-size_image[1]+const.CELL_HEIGHT)*self.scale))
        #else:
        #    pos = (int(((1+2*c_col)*const.CELL_WIDTH/2-size_image[0]+const.CELL_WIDTH)*self.scale), 
        #           int((const.CELL_HEIGHT/2*c_line-size_image[1]+const.CELL_HEIGHT)*self.scale))
        (a,b) = utils.cell_to_point(c_line, c_col, const.ANGLE_X*pi/1800, (const.ANGLE_X+900)*pi/1800, const.CHUNK_GRID_HEIGHT)
        return (int(a+.5),int(b+.5))
    
    def make_grid(self, img_set, cell_ids):
        res = pygame.Surface(self.size, pygame.SRCALPHA)
        res.convert_alpha()
        for c_line in range(len(cell_ids)):
            for c_col in range(len(cell_ids[0])):
                if cell_ids[c_line][c_col] is not None:
                    cell_image = ImageCache.get_image(img_set[cell_ids[c_line][c_col]], self.scale)
                    size_image = cell_image.get_size()
                    pos = self.get_cell_pos(c_line, c_col, size_image)
                    pygame.draw.polygon(res, 0xffffff00,
                                        [utils.cell_to_point(c_line,c_col),utils.cell_to_point(c_line+1,c_col),
                                         utils.cell_to_point(c_line+1,c_col+1),utils.cell_to_point(c_line,c_col+1)],
                                        1)
                    #res.blit(cell_image, pos)
        return res
    
    def update_cell(self, c_line, c_col, new_line, new_col):
        if self.cells[c_line][c_col] is None:
            self.cells[c_line][c_col] = Cell((new_line, new_col), self.img_set, self.cell_ids[new_line][new_col], self.scale)
            if new_line%2 == 0:
                self.cells[c_line][c_col].rect = self.cells[c_line][c_col].rect.move((int(const.CELL_WIDTH*self.scale*c_col), int(const.CELL_HEIGHT*self.scale/2*c_line)))
            else:
                self.cells[c_line][c_col].rect = self.cells[c_line][c_col].rect.move((int(const.CELL_WIDTH*self.scale/2+const.CELL_WIDTH*self.scale*c_col), int(const.CELL_HEIGHT*self.scale/2*c_line)))
        else:
            self.cells[c_line][c_col].default = self.cell_ids[new_line][new_col]
            self.cells[c_line][c_col].index = (new_line, new_col)
            self.cells[c_line][c_col].update_image(self.scale)
    
    def update_grid(self):
        print(self.left_lim_y, self.left_lim_x, "UPDATE")
        line = self.left_lim_y
        col = self.left_lim_x
        for c_line in range(len(self.cells)):
            for c_col in range(len(self.cells[0])):
                self.update_cell(c_line, c_col, line, col)
                col += 1
            col = self.left_lim_x
            line += 1
                
    def zoom(self, dz):
        pass
