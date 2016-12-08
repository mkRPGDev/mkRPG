# -*- coding: utf-8 -*-

import pygame
from pygame.locals import Rect

from interface.backgroundLayer import BackgroundLayer

import const
import interface.utils as utils

class Chunk(pygame.sprite.Sprite):

    def __init__(self, index, cells, map_size):
        pygame.sprite.Sprite.__init__(self)
        self.index = index
        self.cells = cells

        self.g_width = len(self.cells[0])
        self.g_height = len(self.cells)
        self.g_map_width, self.g_map_height = map_size

        self.init_chunk()

    def init_chunk(self):
        # Pas de scale... La taille de chaque chunk est définie par le nb de
        # cellules dans chaque chunk.
        x_min = utils.cell_to_point(self.index[1]*const.CHUNK_GRID_WIDTH,
                                    (self.index[0]+1)*const.CHUNK_GRID_HEIGHT,
                                    self.g_map_width,self.g_map_height,const.CELL_WIDTH,
                                    const.ANGLE_X_R, const.ANGLE_Y_R)[0]
        x_max = utils.cell_to_point((self.index[1]+1)*const.CHUNK_GRID_WIDTH,
                                    self.index[0]*const.CHUNK_GRID_HEIGHT,
                                    self.g_map_width,self.g_map_height,const.CELL_WIDTH,
                                    const.ANGLE_X_R, const.ANGLE_Y_R)[0]
        y_min = utils.cell_to_point((self.index[1]+1)*const.CHUNK_GRID_WIDTH,
                                    (self.index[0]+1)*const.CHUNK_GRID_HEIGHT,
                                    self.g_map_width,self.g_map_height,const.CELL_WIDTH,
                                    const.ANGLE_X_R, const.ANGLE_Y_R)[1]
        y_max = utils.cell_to_point(self.index[1]*const.CHUNK_GRID_WIDTH,
                                    self.index[0]*const.CHUNK_GRID_HEIGHT,
                                    self.g_map_width,self.g_map_height,const.CELL_WIDTH,
                                    const.ANGLE_X_R, const.ANGLE_Y_R)[1]
        self.width = int(x_max-x_min)
        self.height = int(y_max-y_min)
        print("---- ICI ----")
        print(self.index, self.g_map_width, self.g_map_height)
        self.pos = (int(x_min),int(y_min))
        print(self.pos, self.width, self.height)

        self.base_rect = Rect(self.pos, (self.width, self.height))

        self.layers = [BackgroundLayer(self.cells, (self.width, self.height))]
        self.image = self.layers[0].image

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
