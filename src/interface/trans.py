import pygame
from ctypes import *

# gcc -fPIC -std=c99 -shared trans.c -o trans.a
dll = CDLL("./trans.a")

#void trans(int* src, int sw, int sh, int* dst, int dh,
#           double a, double b, double c, double d, int xm, int ym){

def prod(m, p):
    return (p[0]*m[0] + p[1]*m[1], p[0]*m[2] + p[1]*m[3])

def applyMatrix(surf, m):
    """
    Apply m (2x2 matrix) transformation to surf (Pygame surface).
    It is implemented in trans.c to speed up execution.
    Due to a numpy optimisation, pictures with more than 100.000 pixels
    are not supported yet, neither as input nor output.
    Requires about 500us for a 100*100 output.
    """
    if len(m)==2: m=(m[0][0], m[0][1], m[1][0], m[1][1])
    d = m[0]*m[3]-m[1]*m[2]
    if not d: raise ValueError("Non-inversible matrix")
    im = (m[3]/d, -m[1]/d, -m[2]/d, m[0]/d)
    w,h = surf.get_size()
    x1,y1 = prod(m, (w, 0))
    x2,y2 = prod(m, (0, h))
    x3,y3 = prod(m, (w, h))
    xm = min(0, x1, x2, x3)
    ym = min(0, y1, y2, y3)
    xn = max(0, x1, x2, x3)
    yn = max(0, y1, y2, y3)
    ret = pygame.Surface((xn-xm+1, yn-ym+1)).convert_alpha()
    ars = pygame.surfarray.pixels2d(surf)
    ard = pygame.surfarray.pixels2d(ret)
    dll.trans(ars.ctypes.data, len(ars), len(ars[0]), ard.ctypes.data, len(ard), len(ard[0]),
        c_double(im[0]), c_double(im[1]), c_double(im[2]), c_double(im[3]), int(xm), int(ym))
    return ret
