from const import *
from network import NetworkServer
from actions import registerActions
from orders import OrderType

# TODO qui dit mieux ?
with open("isserver.py","w") as file:
    file.write("SERVER = True")
import world

class Server():
    def __init__(self, path):
        self.net = NetworkServer(self.handleEvent)
        self.world = world.World()
        self.world.load(path)
        self.actions = registerActions(PATH)
        
        self.persos = self.world.entities[0] # XXX bricolage

    def __del__(self):
        print("Killing server")
        self.net.kill()

    def run(self):
        self.net.start()
        while 1:
            pass
    
    def handleEvent(self, m):
        ident = m[0]*256+m[1]
        event = m[2:].decode(CODING) # TODO dans network
        for act in self.actions:
            if act.event == event:
                for order in act.orders: # TODO c'est nul -> OrderDispatcher ?
                    if order.type==OrderType.Set and order.args[0]=="emitter":
                        world.BaseObject.ids[ident].treatOrder(order)
                        self.net.broadcast(bytes((ident//256, ident%256)) + order.toBytes())
                        # TODO n'envoyer que les infos non secrètes et en ayant
                        # évalué les formules
#                    elif order.type==OrderType.Create
       
ser = Server(PATH)
ser.run()
