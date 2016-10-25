import curses

from const import *
from network import NetworkClient
import interactions
from orders import Order, OrderType
from time import sleep

#TODO trouver mieux cf world
with open("isserver.py","w") as file:
    file.write("SERVER = False\n")
import world

class Client():
    def __init__(self, path):
        self.net = NetworkClient(self.handleOrder)
        self.win = curses.initscr()
        curses.cbreak()
        self.win.keypad(True)
        self.world = world.loadGame(path)
        self.mv=MapViewer(world.BaseObject.ids[self.world.currentMap], self.world)
        self.interactions = interactions.registerInteractions(PATH)
        
        self.perso = self.world.entities[0] # XXX bricolage

    def __del__(self):
        print("Killing client")
        self.net.kill()

    def run(self):
        self.net.start()
        self.mv.display(self.win)
        while 1:
            k = self.win.getch()
            if k==ord('q'):
                curses.endwin()
                #self.net.kill()
                break
            for inte in self.interactions:
                if (inte.type == interactions.InteractionType.Key and
                    inte.key == k):
                    self.net.sendEvent(self.__getattribute__(inte.target), inte.event)
    
    def handleOrder(self, m):
        ident = m[0]*256+m[1]
        order = Order().fromBytes(m[2:])
        if order.type==OrderType.Set and order.args[0]=="emitter":
            world.BaseObject.ids[ident].treatOrder(order)
        self.mv.display(self.win)
        self.win.addstr(25,0,str(self.world.entities[0].x))
        self.win.refresh()


class CellViewer:
    def __init__(self, cell):
        self.cell = cell
    
    def display(self, win):
        win.addch(self.cell.x, self.cell.y, self.cell.picture)
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
        for c in self.cellViews: c.display(win)
        for ent in self.world.entities:
            win.addch(ent.y, ent.x, ent.picture)

        #win.refresh()

cli = Client(PATH)
try:
    cli.run()
except:
    curses.endwin()
    raise
