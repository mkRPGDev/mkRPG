IMG_PATH = "../content/imgs/"
BG_PATH = "../content/bg/"
GRID_SCALE = 1
SCREEN_WIDTH = 1366
SCREEN_HEIGHT = 768
CELL_WIDTH = 64
CELL_HEIGHT = 33
CHUNK_GRID_WIDTH = 25
CHUNK_GRID_HEIGHT = 50
CHUNK_WIDTH = CHUNK_GRID_WIDTH*CELL_WIDTH
CHUNK_HEIGHT = CHUNK_GRID_HEIGHT*CELL_HEIGHT//2+1
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
