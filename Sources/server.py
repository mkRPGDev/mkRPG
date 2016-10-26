from sys import argv
from const import *
from network import NetworkServer
from actions import registerActions
from orders import OrderDispatcher

# TODO qui dit mieux ?
with open("isserver.py","w") as file:
    file.write("SERVER = True")
import world

class Server():
    def __init__(self, path):
        self.net = NetworkServer(self.handle)
        self.world = world.loadGame(path)
        self.actions = registerActions(path, world.named) # FIXME -> game
        
        self.persos = self.world.entities[0] # XXX bricolage
        self.orderDispatcher = OrderDispatcher(self.world, self.handleEvent)

    def __del__(self):
        print("Killing server")
        self.net.kill()

    def run(self):
        print("Server started")
        self.net.waitForSomeone()
        self.handleEvent(self.world, "start")
        self.net.start()
        while 1: pass
    
    def handle(self, ident, event):
        self.handleEvent(world.BaseObject.ids[ident], event)
    
    def handleEvent(self, emitter, event):
        for act in filter(lambda act: act.event == event, self.actions):
            for order in act.orders:
                if self.orderDispatcher.treat(emitter, order):
                    self.net.sendOrder(emitter.ident, order)
                # TODO n'envoyer que les infos non secrètes et en ayant
                # évalué les formules

ser = Server(argv[1] if len(argv)>1 else PATH)
ser.run()
