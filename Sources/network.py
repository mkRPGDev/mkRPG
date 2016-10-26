from threading import Thread
import socket

from const import *
from orders import Order # XXX à sa place ?

#TODO héritage
class NetworkClient(Thread):
    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.alive = True
        
    def run(self):
        self.soc.bind((HOST, PORT))
        while self.alive:
            msg, addr = self.soc.recvfrom(BUFF)
            ident = msg[0]*256 + msg[1]
            order = Order().fromBytes(msg[2:])
            self.handle(ident, order)
            
    def send(self, m):
        self.soc.sendto(m, (HOST, PORT+1))
    
    def sendEvent(self, obj, eve):
        m = bytes([obj.ident//256, obj.ident%256]) + bytes(eve, CODING)
        self.soc.sendto(m, (HOST, PORT+1))
    
    def kill(self): self.alive = False

class NetworkServer(Thread):
    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.soc.bind((HOST, PORT+1))
        self.alive = True

    def waitForSomeone(self): # XXX temporaire
        msg, addr = self.soc.recvfrom(BUFF)
        assert msg.decode(CODING) == "hello"

    def run(self):
        while self.alive:
            msg, addr = self.soc.recvfrom(BUFF)
            ident = msg[0]*256 + msg[1]
            event = msg[2:].decode(CODING)
            self.handle(ident, event)
    
    def sendOrder(self, ident, order):
        self.broadcast(bytes((ident//256, ident%256)) + order.toBytes())
    
    def send(self, m, dest): self.soc.sendto(m, (HOST, PORT))
    
    def broadcast(self, m): self.soc.sendto(m, (HOST, PORT))

    def kill(self): self.alive = False

