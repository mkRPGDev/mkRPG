from sys import argv
from queue import Queue
from argparse import ArgumentParser

from const import *
from actions import registerActions
from orders import OrderDispatcher
from utils import Perf

if USETCP:
    from network import NetworkServer
else:
    from networkudp import NetworkServer

import world

perf = Perf()

class Server():
    """ Classe principale du processus serveur, concilie réseau, monde, actions et timer """

    def __init__(self, path):
        self.net = NetworkServer(self.handleEvent)
        self.world = world.loadGame(path)
        self.actions = registerActions(path, world.named) # FIXME -> game
        
        self.orderDispatcher = OrderDispatcher(self.world, self.handleEvent)
        self.events = Queue()
        self.pause = False
        
        self.coEntities = {}
        
    def __del__(self):
        print("Killing server")
        self.net.kill()

    def run(self):
        print("Server started; waiting for",len(self.world.entities),"clients")
        self.net.start()
        self.net.waitForClients(len(self.world.entities))
        self.handleEvent(self.world, "start")
        while True:
            emitter, event = self.events.get()
            perf.tic()
            if event == "end": break # TODO gérer la fin
            if event == "pause":
                self.orderDispatcher.timer.pause = True
                self.pause = True
            if self.pause and event == "resume":
                self.orderDispatcher.timer.pause = False
                self.pause = False
                continue
            if self.pause: continue
            if event not in self.actions: continue
            for act in self.actions[event]:
                for order in act.orders:
                    returnOrder = self.orderDispatcher.treat(emitter, order)
                    if returnOrder:
                        self.net.sendOrder(emitter.ident, returnOrder)
                    # TODO n'envoyer que les infos non secrètes
            perf.toc()
        
    def handleEvent(self, emitter, event):
        self.events.put((emitter, event))

parser = ArgumentParser(description="Generic game server.")
parser.add_argument("-p", "--path", default=PATH,
    help="Path of the game directory, should contain game.xml."
    "If this argument is not present, const.py will be used.")
args = parser.parse_args()
ser = Server(args.path)
try:
    ser.run()
except KeyboardInterrupt:
    perf.show()
    print("Exited")
