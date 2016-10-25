from threading import Thread
import socket

from const import *
#import orders

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
            b, a = self.soc.recvfrom(BUFF)           
            self.handle(b)
            
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
        self.alive = True

    def run(self):
        self.soc.bind((HOST, PORT+1))
        print("listening")
        while self.alive:
            b, a = self.soc.recvfrom(BUFF)            
            self.handle(b)
            
    def send(self, m, dest):
        self.soc.sendto(m, (HOST, PORT))
    
    def broadcast(self, m):
        self.soc.sendto(m, (HOST, PORT))

    def kill(self): self.alive = False

