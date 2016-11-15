from threading import Thread
from time import sleep
import socket

from const import *
from orders import Order # XXX à sa place ?

FAM = socket.AF_INET6 if IPV6 else socket.AF_INET

#TODO héritage ?
class NetworkClient(Thread):
    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket.socket(FAM, socket.SOCK_DGRAM)
        self.alive = True
        
    def run(self):
        self.send("hello".encode(CODING))
        while self.alive:
            msg, addr = self.soc.recvfrom(BUFF)
            ident = msg[0]*256 + msg[1]
            order = Order().fromBytes(msg[2:])[0]
            if self.alive:
                self.handle(ident, order)
            
    def send(self, m):
        self.soc.sendto(m, (HOST, PORT))
    
    def sendEvent(self, obj, eve):
        m = bytes([obj.ident//256, obj.ident%256]) + bytes(eve, CODING)
        self.soc.sendto(m, (HOST, PORT))
    
    def kill(self):
        self.soc.close()
        self.alive = False

class NetworkServer(Thread):
    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket.socket(FAM, socket.SOCK_DGRAM)
        self.soc.bind(("", PORT))
        self.alive = True
        self.addr = None

    def waitForClients(self, n): # XXX temporaire
        assert n==1
        if not self.addr:
            msg, self.addr = self.soc.recvfrom(BUFF)
            assert msg.decode(CODING) == "hello"

    def run(self):
        while not self.addr: sleep(0.1)
        while self.alive:
            msg, addr = self.soc.recvfrom(BUFF)
            ident = msg[0]*256 + msg[1]
            event = msg[2:].decode(CODING)
            self.handle(ident, event)
    
    def sendOrder(self, ident, order):
        self.broadcast(bytes((ident//256, ident%256)) + order.toBytes())
    
    def broadcast(self, m): self.soc.sendto(m, self.addr)

    def kill(self): self.alive = False

