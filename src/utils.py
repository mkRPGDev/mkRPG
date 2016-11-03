# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from heapq import heappush, heappop
from math import sqrt

def load_png(name, scale=1):
        """ Load image and return image object"""
        image = pygame.image.load(name)
        if image.get_alpha is None:
                image = image.convert()
        else:
                image = image.convert_alpha()
        width, height = image.get_size()
        if scale != 1:
            image = pygame.transform.scale(image, (int(width*scale), int(height*scale)))
        return image

def add_to_rect_list(list, rect):
    rect_arr = list
    if rect is not None:
        addrect = True
        for rect2 in rect_arr:
            if rect.contains(rect2):
                rect_arr.remove(rect2)
            if rect2.contains(rect):
                addrect = False
        if addrect : rect_arr.append(rect)
    return rect_arr

def merge_rect_lists(list1, list2):
    res = list1
    if list2 is not None:
        for rect in list2:
            res = add_to_rect_list(res, rect)
    return res

def sublist(l, i1, i2, j1, j2):
    return [[l[i][j] for j in range(j1,j2)] for i in range(i1,i2)]
        
class WalkableGraph():
    def __init__(self, walkables):
        self.walkables = walkables
    
    def get_neighbors(self, index):
        u,v = index
        neighbors = [
                     (u-2, v, 1.5),
                     (u-1, v, 1),
                     (u-1, v-1 if u%2 == 0 else v+1, 1),
                     (u, v-1, 1.5),
                     (u, v+1, 1.5),
                     (u+1, v, 1),
                     (u+1, v-1 if u%2 == 0 else v+1, 1),
                     (u+2, v, 1.5),
                    ]
        res = []
        for i,j,c in neighbors:
            if i>-1 and j>-1 and i<len(self.walkables) and\
               j<len(self.walkables[0]) and self.walkables[i][j]:
                res.append((i,j,c))
        return res
        
    def dist(self, u,v):
        x1,y1 = u
        x2,y2 = v
        return sqrt((x2-x1)**2+(y2-y1)**2)
        
    def get_path(self, source, dest):
        d_x, d_y = dest
        
        openCell = []
        costs = {source:0}
        parents = {source:None}
        heappush(openCell, (0, source))
        
        while openCell != []:
            u_heur, u = heappop(openCell)
            if u[0] == d_x and u[1] == d_y:
                res = []
                current = u
                while current is not None:
                    res = [current] + res
                    current = parents[current]
                return res
            for i,j,c in self.get_neighbors(u):
                v = i,j
                new_cost = costs[u]+c
                if v not in costs.keys() or new_cost < costs[v]:
                   costs[v] = new_cost
                   v_heur = costs[v] + self.dist(v, dest)
                   heappush(openCell, (v_heur, v))
                   parents[v] = u
        