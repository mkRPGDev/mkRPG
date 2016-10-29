import curses
from sys import argv
from time import sleep, time

from const import *
from interactions import registerInteractions, InteractionType
from orders import OrderDispatcher

if USETCP:
    from network import NetworkClient
else:
    from networkudp import NetworkClient

#TODO trouver mieux cf world
with open("isserver.py","w") as file:
    file.write("SERVER = False\n")
import world

interface = True # permet de désactiver ncurses pour débugguer

class Client():
    def __init__(self, path):
        self.net = NetworkClient(self.handleOrder)
        self.world = world.loadGame(path)
        if interface:
            self.win = curses.initscr()
            curses.cbreak()
            curses.noecho()
            self.win.keypad(True)
            curses.curs_set(0)
            self.mv=MapViewer(self.world.currentMap, self.world)
        self.interactions = interactions.registerInteractions(path)
        
        self.perso = self.world.entities[0] # XXX bricolage
        self.orderDispatcher = OrderDispatcher(self.world, None)
        self.lastUpdate = 0
        
    def __del__(self):
        self.net.kill()
        curses.endwin()
        print("Client killed")

    def run(self):
        self.net.start()
        if interface: self.mv.display(self.win)
        while True:
            if not interface: continue
            key = self.win.getch()
            if key==ord('q'):
                curses.endwin()
                self.net.kill()
                print("Exited properly")
                break
            for inte in self.interactions:
                if (inte.type == interactions.InteractionType.Key and
                    inte.key == key):
                    self.net.sendEvent(self.__getattribute__(inte.target), inte.event)
    
    def handleOrder(self, ident, order):
        emitter = world.BaseObject.ids[ident]
        self.orderDispatcher.treat(emitter, order)
        if interface and time() - self.lastUpdate > MAXFPS:
            self.mv.display(self.win)
            # TODO insérer ici le xml d'interface
            self.win.addstr(26,0,"Score "+str(self.world.entities[0].score))
            self.win.refresh()
            self.lastUpdate = MAXFPS

class CellViewer:
    def __init__(self, cell):
        self.cell = cell
    
    def display(self, win):
        win.addch(self.cell.y+1, self.cell.x+1, self.cell.picture)

class MapViewer:
    def __init__(self, m, w):
        self.map = m
        self.world = w
        self.cellViews = [CellViewer(c) for c in self.map.cells]
        
    def display(self, win):
        win.clear()
        # image de fond ici
        for x in range(self.map.width+3):
            win.addch(0, x, 35)
            win.addch(self.map.height+2, x, 35)
        for y in range(1, self.map.height+2):
            win.addch(y, 0, 35)
            win.addch(y, self.map.width+2, 35)
        #for c in self.cellViews: c.display(win)
        for ent in self.world.entities:
            win.addch(ent.y+1, ent.x+1, ent.picture)
        for ent in self.world.objects:
            win.addch(ent.y+1, ent.x+1, ent.picture)
        # TODO gérer plusieurs cartes
        
Client(argv[1] if len(argv)>1 else PATH).run()

