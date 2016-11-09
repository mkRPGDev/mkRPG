import curses
from time import sleep, time
from argparse import ArgumentParser

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

class Client():
    """ Classe principale du processus client, concilie interface, monde et réseau """
    """ Main class of the client process, gathering interface, world and networking"""
    def __init__(self, path):
        self.net = NetworkClient(self.handleOrder)
        self.world = world.loadGame(path)
        self.interface = (Curses if interface else Interface)(self.world)
        self.interactions = registerInteractions(path)        
        self.perso = self.world.entities[0] # XXX bricolage
        self.orderDispatcher = OrderDispatcher(self.world, None)
        
    def __del__(self):
        self.net.kill()
        self.interface.end()
        print("Client killed")

    def run(self):
        self.net.start()
        self.interface.update()
        while True:
            key = self.interface.getEvent()
            if key==ord('q'):
                curses.endwin()
                self.net.kill()
                print("Exited properly")
                break
            for inte in self.interactions:
                if (inte.type == InteractionType.Key and
                    inte.key == key):
                    self.net.sendEvent(self.__getattribute__(inte.target), inte.event)
    
    def handleOrder(self, ident, order):
        emitter = world.BaseObject.ids[ident]
        self.orderDispatcher.treat(emitter, order)
        self.interface.update()

class Interface:
    """ Classe mère des UI qui sert également d'implémentation triviale """
    """ UI base-class """
    def __init__(self, w):
        assert type(w) == world.World
        self.world = w
    def update(self): pass
    def end(self): pass
    def getEvent(self):
        while True: sleep(1)

class Curses(Interface):
    """ Implémentation d'interface utilisateur basée sur ncurses """
    def __init__(self, w):
        super().__init__(w)
        self.win = curses.initscr()
        curses.cbreak()
        curses.noecho()
        self.win.keypad(True)
        curses.curs_set(0)
        self.mv=MapViewer(self.world.currentMap, self.world)
        self.lastUpdate = 0

    def update(self):
        if interface and time() - self.lastUpdate > MAXFPS:
            self.mv.display(self.win)
            # TODO insérer ici le xml d'interface
            self.win.addstr(26,0,"Score "+str(self.world.entities[0].score)+'\n')
            self.win.refresh()
            self.lastUpdate = MAXFPS        

    def end(self):
        curses.endwin()

    def getEvent(self):
        return self.win.getch()

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

class CellViewer:
    def __init__(self, cell):
        self.cell = cell
    
    def display(self, win):
        win.addch(self.cell.y+1, self.cell.x+1, self.cell.picture)

parser = ArgumentParser(description="Generic game client.")
parser.add_argument("-p", "--path", default=PATH, help="Path of the game directory, should contain game.xml."
"If this argument is not present, const.py will be used.")
parser.add_argument("-n", "--noui", help="Hide UI for debug purposes", action="store_true")
args = parser.parse_args()
interface = not args.noui
cli = Client(args.path).run()
