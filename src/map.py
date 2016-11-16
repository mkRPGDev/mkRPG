# -*- coding: utf-8 -*-

import pygame
from pygame.locals import MOUSEBUTTONUP

import struct

from utils import merge_rect_lists, sublist, WalkableGraph
from cache import ChunkCache

import const

class MapViewer(pygame.sprite.Group):

    def __init__(self, currentMap, w):
        pygame.sprite.Group.__init__(self)
        self.map = [[currentMap.defaultCell.params["picture"] for i in range(currentMap.width)]\
                     for j in range(currentMap.height)]
        self.world = w
        self.scale = 1

        self.cm_width, self.cm_height = self.load_chunks(self.map)
        self.width = int(currentMap.width*const.CELL_WIDTH*self.scale)
        self.height = int((currentMap.height+2)*const.CELL_HEIGHT/2*self.scale)
        self.walkablesGraph = self.make_walkables(self.map)
        print(currentMap.width, const.CELL_WIDTH, self.width)
        print(currentMap.height, const.CELL_HEIGHT, self.height)

        self.current_chunk = (0,0)
        self.pos_offset = (0,0)
        self.chunk_pos = (0,0)

    def load_chunks(self, bg):
        line = 0
        col = 0

        map_width = len(bg[0])
        map_height = len(bg)

        chunks = []

        while (line+1)*const.CHUNK_GRID_HEIGHT <= map_height:
            while (col+1)*const.CHUNK_GRID_WIDTH <= map_width:
                cells = sublist(bg,
                                line*const.CHUNK_GRID_HEIGHT,
                                (line+1)*const.CHUNK_GRID_HEIGHT,
                                col*const.CHUNK_GRID_WIDTH,
                                (col+1)*const.CHUNK_GRID_WIDTH)
                chunks.append(((line, col), cells,
                              (self.cm_width,self.cm_height)))
                col += 1
            if col*const.CHUNK_GRID_WIDTH < map_width:
                cells = sublist(bg,
                                line*const.CHUNK_GRID_HEIGHT,
                                (line+1)*const.CHUNK_GRID_HEIGHT,
                                col*const.CHUNK_GRID_WIDTH,
                                map_width)
                chunks.append(((line,col), cells,
                              (self.cm_width,self.cm_height)))
            line += 1
            col = 0

        if line*const.CHUNK_GRID_HEIGHT < map_height:
            print(col)
            while (col+1)*const.CHUNK_GRID_WIDTH <= map_width:
                cells = sublist(bg,
                                line*const.CHUNK_GRID_HEIGHT,
                                map_height,
                                col*const.CHUNK_GRID_WIDTH,
                                (col+1)*const.CHUNK_GRID_WIDTH)
                chunks.append(((line, col), cells))
                col += 1
            if col*const.CHUNK_GRID_WIDTH < map_width:
                cells = sublist(bg,
                                line*const.CHUNK_GRID_HEIGHT,
                                map_height,
                                col*const.CHUNK_GRID_WIDTH,
                                map_width)
                chunks.append(((line,col), cells))

        self.chunks_state = ChunkCache.init_chunks(chunks)

        return len(self.chunks_state[0]), len(self.chunks_state)

    def make_walkables(self, bg):
        return WalkableGraph([[True for c in c_line] for c_line in bg])

    def zoom(self, dz):
        pass

    def move(self, dx, dy):
        predicted_new_pos = (self.pos_offset[0]+dx, self.pos_offset[1]+dy)

        if predicted_new_pos[0] < const.MOV_OFFSET and\
           predicted_new_pos[0] > -self.width+const.SCREEN_WIDTH-const.MOV_OFFSET:
           new_pos_x = predicted_new_pos[0]
        else:
           new_pos_x = self.pos_offset[0]

        if predicted_new_pos[1] < const.MOV_OFFSET and\
           predicted_new_pos[1] > -self.height+const.SCREEN_HEIGHT-const.MOV_OFFSET:
           new_pos_y = predicted_new_pos[1]
        else:
           new_pos_y = self.pos_offset[1]

        self.current_chunk = (max(-new_pos_y//(const.CHUNK_HEIGHT*self.scale),0),
                              max(-new_pos_x//(const.CHUNK_WIDTH*self.scale),0))

        self.pos_offset = (new_pos_x, new_pos_y)
        self.chunk_pos = (-(-new_pos_x%const.CHUNK_WIDTH),
                          -(-new_pos_y%const.CHUNK_HEIGHT))
        print(self.pos_offset, self.chunk_pos)

    def render(self):
        sprites = self.sprites()
        rect = sprites[0].rect
        size = (3*const.CHUNK_WIDTH,
                3*const.CHUNK_HEIGHT)

        res = pygame.Surface(size)
        res.convert_alpha()
        res.fill((0,0,0))
        offsetx,offsety = (self.current_chunk[0]*const.CHUNK_WIDTH*self.scale,
                           self.current_chunk[1]*const.CHUNK_HEIGHT*self.scale)

        for sprite in self.sprites():
            sprite.render()
            sprite.rect.move((-offsetx, -offsety))


        self.draw(res)

        for sprite in self.sprites():
            sprite.rect.move((offsetx, offsety))

        print(self.current_chunk)

        return res

    def neighbors_chunk(self, chunk):
        """ Returns indexes of the neighbors of chunk and index of chunk """
        res = []
        for i in range(-1,2):
           for j in range(-1,2):
               line = chunk[0]+i
               col = chunk[1]+j
               if line >= 0 and line < self.cm_height:
                   if col >= 0 and col < self.cm_width:
                       res.append((line,col))
        return res

    def onscreen_chunks(self):
        return [index for index in self.neighbors_chunk(self.current_chunk)]

    def update(self):
        self.empty()
        mouse_pos = pygame.mouse.get_pos()
        rel_m_pos = (mouse_pos[0]-self.pos_offset[0],
                     mouse_pos[1]-self.pos_offset[1])
        rect_arr = []
        for line,col in self.onscreen_chunks():
            chunk = ChunkCache.get_chunk((line,col), self.scale)

            if chunk.rect.collidepoint(rel_m_pos):
                m_pos_upd = rel_m_pos
            else:
                m_pos_upd = None

            self.chunks_state[line][col], rects = chunk.update(self.chunks_state[line][col], m_pos_upd)
            rect_arr = merge_rect_lists(rect_arr, rects)
            self.add(chunk)
        for i in range(len(rect_arr)):
            rect_arr[i] = rect_arr[i].move(self.pos_offset)
        return rect_arr

    def propagate_trigger(self, event):
        if event.type == MOUSEBUTTONUP and event.button == 1:
            mouse_pos = pygame.mouse.get_pos()
            rel_m_pos = (mouse_pos[0]-self.pos_offset[0],
                         mouse_pos[1]-self.pos_offset[1])

            for line,col in self.onscreen_chunks():
                chunk = ChunkCache.get_chunk((line,col), self.scale)

                if chunk.rect.collidepoint(rel_m_pos):
                    return chunk.click_trigger(rel_m_pos)
        return ''

    def compute_path(self, start_pos, end_pos):
        return self.walkablesGraph.get_path(start_pos, end_pos)

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
