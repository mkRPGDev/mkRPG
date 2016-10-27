import curses
from sys import argv
from const import *
from network import NetworkClient
import interactions
from orders import OrderDispatcher
from time import sleep

#TODO trouver mieux cf world
with open("isserver.py","w") as file:
    file.write("SERVER = False\n")
import world

interface = True

class Client():
    def __init__(self, path):
        self.net = NetworkClient(self.handleOrder)
        if interface:
            self.win = curses.initscr()
            curses.cbreak()
            curses.noecho()
            self.win.keypad(True)
        self.world = world.loadGame(path)
        self.mv=MapViewer(self.world.currentMap, self.world)
        self.interactions = interactions.registerInteractions(path)
        
        self.perso = self.world.entities[0] # XXX bricolage
        self.orderDispatcher = OrderDispatcher(self.world, None)
        
    def __del__(self):
        self.net.kill()
        curses.endwin()
        print("Client killed")

    def run(self):
        self.net.start()
        if interface: self.mv.display(self.win)
        self.net.send("hello".encode(CODING))
        
        while 1:
            if not interface: continue
            k = self.win.getch()
            if k==ord('q'):
                curses.endwin()
                self.net.kill()
                print("Exited properly")
                break
            if k==ord('b'):
                1/0
            for inte in self.interactions:
                if (inte.type == interactions.InteractionType.Key and
                    inte.key == k):
                    self.net.sendEvent(self.__getattribute__(inte.target), inte.event)
    
    def handleOrder(self, ident, order):
        emitter = world.BaseObject.ids[ident]
        self.orderDispatcher.treat(emitter, order)
        # TODO limiter les actualisations
        if interface:
            self.mv.display(self.win)
            # TODO insérer ici le xml d'interface
            self.win.addstr(26,0,"Score "+str(self.world.entities[0].score))
            self.win.refresh()

class CellViewer:
    def __init__(self, cell):
        self.cell = cell
    
    def display(self, win):
        win.addch(self.cell.y+1, self.cell.x+1, self.cell.picture)
#        for ent in self.cell.entities:
#            win.addch(self.cell.x, self.cell.y, ent.picture)
#        for obj in self.cell.objects:
#            win.addch(self.cell.x, self.cell.y, obj.picture)

class MapViewer:
    def __init__(self, m, w):
        self.map = m
        self.world = w
        self.cellViews = [CellViewer(c) for c in self.map.cells]
        
    def display(self, win):
        win.clear()
        #image de fond
        
        for x in range(self.map.width+3):
            win.addch(0, x, 35)
            win.addch(self.map.height+2, x, 35)
        for y in range(1, self.map.height+2):
            win.addch(y, 0, 35)
            win.addch(y, self.map.width+2, 35)
        #items
        for c in self.cellViews: c.display(win)
        for ent in self.world.entities:
            win.addch(ent.y+1, ent.x+1, ent.picture)
        for ent in self.world.objects:
            win.addch(ent.y+1, ent.x+1, ent.picture)
        #win.refresh()

cli = Client(argv[1] if len(argv)>1 else PATH)
try:
    cli.run()
except:
    del cli
    raise
