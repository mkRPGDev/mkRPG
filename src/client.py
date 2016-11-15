from time import sleep, time
from argparse import ArgumentParser

from const import *
from interactions import registerInteractions, InteractionType
from orders import OrderDispatcher

if USETCP:
    from network import NetworkClient
else:
    from networkudp import NetworkClient

import world

def interface(args):
    """ Import the correct interface according to user choice """
    if args.curses:
        from cursescli import Curses as Interface
    elif args.noui:
        from interface import Interface
    elif args.pygame:
        from pygamecli import A as Interface
    else:
        # choix par défaut
        args.curses = True
        return interface(args)
    return Interface

class Client():
    """ Main class of the client process, gathering interface, world and networking"""
    def __init__(self, path, Interface):
        self.net = NetworkClient(self.handleOrder)
        self.world = world.loadGame(path)
        self.interface = Interface(self.world)
        self.interactions = registerInteractions(path)        
        self.perso = None
        self.orderDispatcher = OrderDispatcher(self.world, None)
        print(self.world.entities)
        
    def __del__(self):
        self.net.kill()
        self.interface.end()
        print("Client killed")

    def run(self):
        for ent in self.world.entities:
            if self.net.askEntity(ent):
                self.perso = ent
                break
        else:
            print("Aucun perso disponible !")
            return
        self.net.start()
        self.interface.update()
        while True:
            key = self.interface.getEvent()
            if key==ord('q'):
                curses.endwin()
                self.net.kill()
                print("Exited properly")
                break
            elif key==ord('p'): self.net.sendEvent(self.world, "pause")
            # TODO relayer l'affichage
            elif key==ord('r'): self.net.sendEvent(self.world, "resume")
            for inte in self.interactions:
                if (inte.type == InteractionType.Key and
                    inte.key == key):
                    self.net.sendEvent(self.__getattribute__(inte.target), inte.event)
    
    def handleOrder(self, ident, order):
        emitter = world.BaseObject.ids[ident]
        self.orderDispatcher.treat(emitter, order)
        self.interface.update()

parser = ArgumentParser(description="Generic game client.")
parser.add_argument("-p", "--path", default=PATH,
                    help="Path of the game directory, should contain game.xml."
                    "If this argument is not present, const.py will be used.")

uimode = parser.add_mutually_exclusive_group()
uimode.add_argument("-c", "--curses", action="store_true",
                    help="Use ncurses (requires Unix) instead of PyGame.")
uimode.add_argument("-g", "--pygame", action="store_true",
                    help="Use PyGame interface. (default)")
uimode.add_argument("-n", "--noui", action="store_true",
                    help="Hide UI for debug purposes")

args = parser.parse_args()
cli = Client(args.path, interface(args)).run()

