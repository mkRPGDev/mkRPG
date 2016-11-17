IMG_PATH = "../content/imgs/"
BG_PATH = "../content/bg/"
GRID_SCALE = 1
SCREEN_WIDTH = 1366
SCREEN_HEIGHT = 768
CELL_WIDTH = 64


CELL_HEIGHT = 33
# Inutile : les cellules sont carrées et la forme
# déterminée par les angles

ANGLE_X = 0
ANGLE_Y = 0
# Angles en dixième de degré
from math import pi
deg_to_rad = lambda a : a*pi/1800
ANGLE_X_R = deg_to_rad(ANGLE_X)
ANGLE_Y_R = deg_to_rad(ANGLE_Y+900)
# Angles en radian


CHUNK_GRID_WIDTH = 5
CHUNK_GRID_HEIGHT = 5

import utils
CHUNK_WIDTH = int(utils.cell_to_point(CHUNK_GRID_WIDTH,0,
                                      CHUNK_GRID_WIDTH,CHUNK_GRID_HEIGHT,
                                      CELL_WIDTH,ANGLE_X_R,ANGLE_Y_R)[0]-
                  utils.cell_to_point(0,CHUNK_GRID_HEIGHT,
                                      CHUNK_GRID_WIDTH,CHUNK_GRID_HEIGHT,
                                      CELL_WIDTH,ANGLE_X_R,ANGLE_Y_R)[0])
CHUNK_HEIGHT = int(utils.cell_to_point(0,0,CHUNK_GRID_WIDTH,CHUNK_GRID_HEIGHT,
                                       CELL_WIDTH,ANGLE_X_R,ANGLE_Y_R)[1]-
                   utils.cell_to_point(CHUNK_GRID_WIDTH,CHUNK_GRID_HEIGHT,
                                       CHUNK_GRID_WIDTH,CHUNK_GRID_HEIGHT,
                                       CELL_WIDTH,ANGLE_X_R,ANGLE_Y_R)[1])


BG_IMGSET = {1:"ground.png", 2:"grass.png", 32:"ground.png"}
GRID_IMGSET = {"default":"cell.png", "selected":"cell_selected.png",
               "mouse_iso":"cell_mouse_iso.png"}
CELL_MATCH = {1:"default", 2:"default", 32:"default", None:None}
MOV_OFFSET = 40
CHAR_MOV_SPEED = 10 # pixels/second
CHAR_ANIM_SPEED = 1 # frames between images
MAX_CACHE_SIZE = 50
MIN_ZOOM = 0.9
MAX_ZOOM = 1.3
ZOOM_STEP = 0.1

VERBOSE = False

# ! PATH par défaut en l'absence d'argument
PATH = "../test/snake/" # et on oublie pas le dernier /

MAXFPS = 60
UPDTIME = 1/MAXFPS

# Réseau
HOST = "localhost"
#HOST = "paquebot.ens.fr"
PORT = 12345
IPV6 = False#True
USETCP = True
BUFF = 1024
CODING = "ascii"
