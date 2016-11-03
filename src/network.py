from threading import Thread
from socket import socket, SOCK_STREAM, AF_INET, AF_INET6, SOL_SOCKET, SO_REUSEADDR
from time import sleep

from const import *
from orders import Order # XXX à sa place ?

class NetworkClient(Thread):
    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket(AF_INET6 if IPV6 else AF_INET, SOCK_STREAM)
        self.alive = True
        
    def run(self):
        self.soc.connect((HOST, PORT))
        while self.alive:
            msg = self.soc.recv(BUFF)
            ident = msg[0]*256 + msg[1]
            order = Order().fromBytes(msg[2:])
            if self.alive:
                self.handle(ident, order)
            
    def send(self, m):
        self.soc.sendall(m)
    
    def sendEvent(self, obj, eve):
        m = bytes([obj.ident//256, obj.ident%256]) + bytes(eve, CODING)
        self.send(m)
    
    def kill(self):
        self.soc.close()
        self.alive = False

class ServerConnection(Thread):
    def __init__(self, soc, han):
        Thread.__init__(self)
        self.soc = soc
        self.handle = han
    
    def run(self):
        while True:
            msg = self.soc.recv(BUFF)
            ident = msg[0]*256 + msg[1]
            event = msg[2:].decode(CODING)
            self.handle(ident, event)
            
    def send(self, m):
        self.soc.sendall(m)

class NetworkServer(Thread):
    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket(AF_INET6 if IPV6 else AF_INET, SOCK_STREAM)
        self.soc.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1) # <3
        self.soc.bind(("", PORT))
        self.alive = True
        self.co = []

    def waitForClients(self, n):
        while len(self.co)<n: sleep(0.1)

    def run(self):
        self.soc.listen()
        while self.alive:
            soc, addr = self.soc.accept()
            print(addr, "connected")
            co = ServerConnection(soc, self.handle)
            self.co.append(co)
            co.start()
    
    def sendOrder(self, ident, order):
        self.broadcast(bytes((ident//256, ident%256)) + order.toBytes())
    
    #def send(self, m, dest): self.soc.sendto(m, (HOST, PORT))
    
    def broadcast(self, m):
        for co in self.co:
            co.send(m)

    def kill(self): self.alive = False

