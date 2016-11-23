import pygame

from heapq import heappush, heappop
from math import sqrt, cos, sin, pi

def load_png(name, scale=1):
        """ Load image and return image object"""
        image = pygame.image.load(name)
        if image.get_alpha is None:
                image = image.convert()
        else:
                image = image.convert_alpha()
        width, height = image.get_size()
        if scale != 1:
            image = pygame.transform.scale(image, (int(width*scale),
                                                   int(height*scale)))
        return image

def add_to_rect_list(list, rect):
    """
    Rect is a Pygame type representing a rectangle (x,y,l,w).
    This function update a list, ignoring overlapping objects.
    """
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
    """ Merge two lists of rect as described above. """
    res = list1
    if list2 is not None:
        for rect in list2:
            res = add_to_rect_list(res, rect)
    return res

def sublist(l, i1, i2, j1, j2):
    return [[l[i][j] for j in range(j1,j2)] for i in range(i1,i2)]

class WalkableGraph():
    def __init__(self, walkables):
        """
        Walkables is a bit array representing cells that can be crossed
        during a move
        """
        self.walkables = walkables

    def get_neighbors(self, index):
        """
        Return accessible cells around index, with a cost value depending
        on the direction of the move.
        """
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
        """ Return distance between two nodes on the grid """
        x1,y1 = u
        x2,y2 = v
        return sqrt((x2-x1)**2+(y2-y1)**2)

    def get_path(self, source, dest):
        """ A* algorithm on the current graph """
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

def cell_to_point(x, y, nb_cells_x, nb_cells_y, cell_width,
                  angle_x, angle_y):
    """ Convert cell related coordinates to map coordinates

    \warning Angles must be given in radian.
    """
    return ((( x            *cos(angle_x) - (nb_cells_y-y)*cos(angle_y))*cell_width),
            (((nb_cells_x-x)*sin(angle_x) + (nb_cells_y-y)*sin(angle_y))*cell_width))

def point_to_cell(x, y, nb_cells_x, nb_cells_y, cell_width,
                  angle_x, angle_y):
    """ Convert map coordinates to cell related coordinates 

    \warning Angles must be given in radian.
    """
    d = -cos(angle_x)*sin(angle_y)+sin(angle_x)*cos(angle_y)
    x += nb_cells_y*cos(angle_y)*cell_width
    y += -(nb_cells_x*sin(angle_x)+nb_cells_y*sin(angle_y))*cell_width
    # nb_cell_y + 2 ?
    return (((-sin(angle_y)*x -cos(angle_y)*y)/d/cell_width),
            ((+sin(angle_x)*x +cos(angle_x)*y)/d/cell_width))

def testCoord(x,y,angx,angy):
    # fonction de test des changement de coordonnées.
    angy += 900
    deg_to_rad = lambda a : a*pi/1800
    (a,b) = point_to_cell(x,y,42,42,64, deg_to_rad(angx),deg_to_rad(angy))
    (c,d) = cell_to_point(x,y,42,42,64, deg_to_rad(angx),deg_to_rad(angy))
    print("pt -> cl -> pt", (x,y), cell_to_point(a,b,42,42,64, deg_to_rad(angx),deg_to_rad(angy)))
    print("cl -> pt -> cl", (x,y), point_to_cell(c,d,42,42,64, deg_to_rad(angx),deg_to_rad(angy)))
