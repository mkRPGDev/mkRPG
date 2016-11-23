# -*- coding: utf-8 -*-

import pygame

from cache import ImageCache
import const
import utils
from math import pi

class Layer(pygame.sprite.Sprite):
    """ A abstract class defining a layer of the display (ex. background) """

    def __init__(self, size):
        pygame.sprite.Sprite.__init__(self)
        self.cells = []
        self.g_width = 0
        self.g_height = 0
        self.size = size

    def render(self, surf):
        pass

    def update(self):
        pass

    def get_cell_pos(self, c_line, c_col, size_image):
        (a,b) = utils.cell_to_point(c_line, c_col, const.CHUNK_GRID_WIDTH,
                                    const.CHUNK_GRID_HEIGHT, const.CELL_WIDTH,
                                    const.ANGLE_X_R, const.ANGLE_Y_R)
        return (int(a+.5),int(b+.5))

    def make_grid(self, img_set, cell_ids):
        """
        Build the grid that will be drawn by Pygame. Img_set is a list
        of (id/type, filename) specifying the files to load for each type
        of cell and cell_ids is the type for each cell.
        """
        res = pygame.Surface(self.size, pygame.SRCALPHA)
        res.convert_alpha()
        # Debug : fond de chunk aléatoire
        import random
        res.fill((random.randint(0,0),random.randint(0,0),random.randint(0,255),255))
        # Fin debug
        for c_line in range(len(cell_ids)):
            for c_col in range(len(cell_ids[0])):
                if cell_ids[c_line][c_col] is not None:
                    cell_image = ImageCache.get_image(img_set[cell_ids[c_line][c_col]])
                    size_image = cell_image.get_size()
                    pos = self.get_cell_pos(c_line, c_col, size_image)
                    pygame.draw.polygon(res, (c_line*40,c_col*40,0,255),
                                        [utils.cell_to_point(c_line,c_col,self.g_width,self.g_height,const.CELL_WIDTH,const.ANGLE_X_R,const.ANGLE_Y_R),
                                         utils.cell_to_point(c_line+1,c_col,self.g_width,self.g_height,const.CELL_WIDTH,const.ANGLE_X_R,const.ANGLE_Y_R),
                                         utils.cell_to_point(c_line+1,c_col+1,self.g_width,self.g_height,const.CELL_WIDTH,const.ANGLE_X_R,const.ANGLE_Y_R),
                                         utils.cell_to_point(c_line,c_col+1,self.g_width,self.g_height,const.CELL_WIDTH,const.ANGLE_X_R,const.ANGLE_Y_R)],
                                        1)
        return res
