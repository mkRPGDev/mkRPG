import pygame
from pygame.locals import MOUSEBUTTONUP, Rect

import struct

from interface.utils import merge_rect_lists, sublist, WalkableGraph
from interface.cache import ChunkCache

import interface.const as const

class MapViewer(pygame.sprite.Group):

    def __init__(self, currentMap, w):
        pygame.sprite.Group.__init__(self)
        self.map = [[currentMap.defaultCell.picture for i in range(currentMap.width)]\
                     for j in range(currentMap.height)]
        self.world = w

        self.cm_width, self.cm_height = self.load_chunks(self.map)
        self.width = int(currentMap.width*const.CELL_WIDTH)
        self.height = int((currentMap.height+2)*const.CELL_WIDTH/2)
        self.screen_cgwidth = const.SCREEN_WIDTH//const.CHUNK_WIDTH + 3
        self.screen_cgheight = const.SCREEN_HEIGHT//const.CHUNK_HEIGHT + 3
        self.walkablesGraph = self.make_walkables(self.map)

        self.last_curr_chunk = (-1,-1)
        self.current_chunk = (self.cm_height-1,0)
        self.pos_offset = (0,0)
        self.chunk_pos = (0,0)

        self.update()

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
                              (len(self.map[0]), len(self.map))))
                col += 1
            if col*const.CHUNK_GRID_WIDTH < map_width:
                cells = sublist(bg,
                                line*const.CHUNK_GRID_HEIGHT,
                                (line+1)*const.CHUNK_GRID_HEIGHT,
                                col*const.CHUNK_GRID_WIDTH,
                                map_width)
                chunks.append(((line,col), cells,
                              (len(self.map[0]), len(self.map))))
            line += 1
            col = 0

        if line*const.CHUNK_GRID_HEIGHT < map_height:
            while (col+1)*const.CHUNK_GRID_WIDTH <= map_width:
                cells = sublist(bg,
                                line*const.CHUNK_GRID_HEIGHT,
                                map_height,
                                col*const.CHUNK_GRID_WIDTH,
                                (col+1)*const.CHUNK_GRID_WIDTH)
                chunks.append(((line, col), cells,
                              (len(self.map[0]), len(self.map))))
                col += 1
            if col*const.CHUNK_GRID_WIDTH < map_width:
                cells = sublist(bg,
                                line*const.CHUNK_GRID_HEIGHT,
                                map_height,
                                col*const.CHUNK_GRID_WIDTH,
                                map_width)
                chunks.append(((line,col), cells,
                              (len(self.map[0]), len(self.map))))

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

        self.current_chunk = (max(self.cm_height-1+\
                              new_pos_y//(const.CHUNK_HEIGHT),0),
                              max(-new_pos_x//(const.CHUNK_WIDTH),0))

        self.pos_offset = (new_pos_x,new_pos_y)
        self.chunk_pos = (new_pos_x if new_pos_x > 0 else -(-new_pos_x%const.CHUNK_WIDTH),
                          new_pos_y if new_pos_y > 0 else -(-new_pos_y%const.CHUNK_HEIGHT))
        #self.chunk_pos = self.pos_offset
        #print(self.pos_offset)

    def render(self):
        rect = Rect((0,0),(0,0))
        rect = rect.unionall([chunk.rect for chunk in self.sprites()])
        size = rect.size

        res = pygame.Surface(size)
        res = res.convert_alpha()
        res.fill((0,0,0))
        pygame.draw.rect(res, (255,255,255,255), Rect((0,0), size),10)

        self.draw(res)

        return res

    def onscreen_chunks(self):
        res = []
        for i in range(-1,self.screen_cgwidth):
            if self.current_chunk[0]-i >= 0 and\
               self.current_chunk[0]-i < self.cm_height:
                for j in range(-1,self.screen_cgheight):
                    if self.current_chunk[1]+j < self.cm_width and\
                       self.current_chunk[1]+j >= 0:
                        res.append((self.current_chunk[0]-i,
                                   self.current_chunk[1]+j))
        return res

    def update(self):
        mouse_pos = pygame.mouse.get_pos()
        rel_m_pos = (mouse_pos[0]-self.pos_offset[0],
                     mouse_pos[1]-self.pos_offset[1])

        rect_arr = self.update_chunks()

        for chunk in self.sprites():
            if chunk.rect.collidepoint(rel_m_pos):
                m_pos_upd = rel_m_pos
            else:
                m_pos_upd = None

            line,col = chunk.index
            self.chunks_state[line][col], rects = chunk.update(
                                                self.chunks_state[line][col],
                                                m_pos_upd)
            rect_arr = merge_rect_lists(rect_arr, rects)

    def update_chunks(self):
        if self.last_curr_chunk != self.current_chunk:
            self.empty()
            self.last_curr_chunk = self.current_chunk

            rect_arr = []

            pos_offsetx,pos_offsety = ChunkCache.get_chunk(self.current_chunk).pos
            pos_offsetx *= -1
            pos_offsety *= -1

            curr_line,curr_col = self.current_chunk
            if curr_line < self.cm_height-1:
                pos_offsety += const.CHUNK_HEIGHT
            if curr_col > 0:
                pos_offsetx += const.CHUNK_WIDTH

            for line,col in self.onscreen_chunks():
                chunk = ChunkCache.get_chunk((line,col))
                chunk.rect = chunk.base_rect.move(pos_offsetx,pos_offsety)
                self.add(chunk)

            for i in range(len(rect_arr)):
                rect_arr[i] = rect_arr[i].move(self.pos_offset)

            print("Current chunk",self.current_chunk)
            print("Offset", pos_offsetx, pos_offsety)
            #print("Screen chunks")
            #for chunk in self.sprites():
            #    print(chunk.index,":", str(chunk.rect), str(chunk.base_rect))

            return rect_arr
        else:
            return []

    def propagate_trigger(self, event):
        if event.type == MOUSEBUTTONUP and event.button == 1:
            mouse_pos = pygame.mouse.get_pos()
            rel_m_pos = (mouse_pos[0]-self.pos_offset[0],
                         mouse_pos[1]-self.pos_offset[1])

            for line,col in self.onscreen_chunks():
                chunk = ChunkCache.get_chunk((line,col))

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
