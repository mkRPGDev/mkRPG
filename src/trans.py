import pygame
from ctypes import *
from numpy import array

# gcc -fPIC -std=c99 -shared trans.c -o trans.a
dll = CDLL("./trans.a")

#void trans(int* src, int sw, int sh, int* dst, int dh,
#           double a, double b, double c, double d, int xm, int ym){

def prod(m, p):
    return (p[0]*m[0] + p[1]*m[1], p[0]*m[2] + p[1]*m[3]) 

def transfo(surf, m):
    # nécessite 70us pour 100*100
    w,h = surf.get_size()
    x1,y1 = prod(m, (w, 0))
    x2,y2 = prod(m, (0, h))
    x3,y3 = prod(m, (w, h))
    xm = min(0, x1, x2, x3)
    ym = min(0, y1, y2, y3)
    xn = max(0, x1, x2, x3)
    yn = max(0, y1, y2, y3)
    ret = pygame.Surface((xn-xm+1, yn-ym+1))
    ars = pygame.surfarray.pixels2d(surf)
    ard = pygame.surfarray.pixels2d(ret)

    #src = (POINTER(c_int) * w)(*map(lambda x:(c_int * h)(*x), ars))    
    #dst = (POINTER(c_int) * len(ard))(*map(lambda x:(c_int * len(ard[0]))(*x), ard))    
    dll.trans(ars.ctypes.data, w, h, ard.ctypes.data, len(ard[0]),
        c_double(m[0]), c_double(m[1]), c_double(m[2]), c_double(m[3]), xm, ym)

    return ret

