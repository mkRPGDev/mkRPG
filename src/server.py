from sys import argv
from queue import Queue

from const import *
from actions import registerActions
from orders import OrderDispatcher
from utils import Perf

if USETCP:
    from network import NetworkServer
else:
    from networkudp import NetworkServer

# TODO qui dit mieux ?
with open("isserver.py","w") as file:
    file.write("SERVER = True")
import world

perf = Perf()

class Server():
    def __init__(self, path):
        self.net = NetworkServer(self.handle)
        self.world = world.loadGame(path)
        self.actions = registerActions(path, world.named) # FIXME -> game
        
        self.persos = self.world.entities[0] # XXX bricolage
        self.orderDispatcher = OrderDispatcher(self.world, self.handleEvent)
        self.events = Queue()

    def __del__(self):
        print("Killing server")
        self.net.kill()

    def run(self):
        print("Server started")
        self.net.start()
        self.net.waitForClients(1)
        self.handleEvent(self.world, "start")
        while True:
            emitter, event = self.events.get()
            perf.tic()
            if event == "end": break # TODO gérer la fin
            for act in self.actions[event]:
                for order in act.orders:
                    returnOrder = self.orderDispatcher.treat(emitter, order)
                    if returnOrder:
                        self.net.sendOrder(emitter.ident, returnOrder)
                    # TODO n'envoyer que les infos non secrètes
            perf.toc()
    
    def handle(self, ident, event):
        self.handleEvent(world.BaseObject.ids[ident], event)
    
    def handleEvent(self, emitter, event):
        self.events.put((emitter, event))

ser = Server(argv[1] if len(argv)>1 else PATH)
try:
    ser.run()
except KeyboardInterrupt:
    perf.show()
    print("Exited")