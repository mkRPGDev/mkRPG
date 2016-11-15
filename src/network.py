from threading import Thread
from socket import socket, timeout, \
    SOCK_STREAM, AF_INET, AF_INET6, SOL_SOCKET, SO_REUSEADDR
from time import sleep

from const import *
from world import BaseObject
from orders import Order # XXX à sa place ?

class NetworkClient(Thread):
    """
    This class manages the network thread of the client.  It allows the client
    to send messages (describing events) to the server.
    """

    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket(AF_INET6 if IPV6 else AF_INET, SOCK_STREAM)
        self.alive = True
        self.soc.connect((HOST, PORT))

    def askEntity(self, ent):
        """ Ask for an entity """
        self.sendEvent(ent, "acquire")
        msg = self.soc.recv(BUFF) # TODO plusieurs messages
        return msg == b"accepted"
                
    
    def run(self):
        """
        Main loop of the client's network thread.  After connecting the socket
        to the server, wait for orders and handle them immediately until
        connection ends.
        """
        while self.alive:
            msg = self.soc.recv(BUFF)
            if not msg:
                print("Connection lost\n")
                self.alive = False
            size = 0
            while size < len(msg):
                ident = msg[size]*256 + msg[size+1]
                order, i = Order().fromBytes(msg[size+2:])
                if self.alive:
                    self.handle(ident, order) # TODO casser l'asymétrie
                size += i + 2

    def send(self, m):
        self.soc.sendall(m)

    def sendEvent(self, obj, event):
        """
        Send an event to the server in a formatted way, specifying the id
        of the object to affect.
        """
        assert(len(event)<256)
        m = bytes([obj.ident//256, obj.ident%256, len(event)]) + \
            bytes(event, CODING)
        self.send(m)

    def kill(self):
        self.soc.close()
        self.alive = False

class ServerConnection(Thread):
    """
    This thread manages the communications with one particular client (one
    thread is created by client).
    """
    def __init__(self, soc, handle, parent):
        Thread.__init__(self)
        self.soc = soc
        self.soc.settimeout(1)
        self.handle = handle
        self.entity = None
        self.server = parent
    
    def run(self):
        """
        Wait for messages from the client and handle them immediately.
        """
        while True:
            try:
                msg = self.soc.recv(BUFF)
            except timeout:
                continue
            except OSError as err:
                if err.errno in (9, 104): return
                # bad file desc or connection reset by peer
                raise
            while msg:
                ident = msg[0]*256 + msg[1]
                length = msg[2]
                event = msg[3:3+length].decode(CODING)
                assert ident in BaseObject.ids
                emitter = BaseObject.ids[ident]
                if self.entity: # TODO and self.entity.ident == ident:
                    self.handle(emitter, event)
                else:
                    assert event == "acquire"
                    if not emitter.user: # TODO opération atomique
                        emitter.user = self
                        self.entity = emitter
                        self.handle(emitter, "init")
                        self.send(b"accepted")
                    else:
                        self.send(b"rejected")
                    
                msg = msg[3+length:]
                
    def send(self, m): # TODO gérer les envois avec le thread d'écoute
        try:
            self.soc.sendall(m)
        except BrokenPipeError:
            self.end()# le recv va se charger de fermer la connection
            pass
            
    def end(self):
        print("Déco")
        if self.entity: self.entity.user = None
        self.soc.close()
        self.server.connections.remove(self)
        

class NetworkServer(Thread):
    """
    This class is the thread of the server that manages clients connections on
    his port.  It keeps the list of connected clients and provides method to
    broadcast messages to all the clients.
    """

    def __init__(self, handle):
        """ The server listen on the port specified in const.py. """
        Thread.__init__(self)
        self.handle = handle
        self.soc = socket(AF_INET6 if IPV6 else AF_INET, SOCK_STREAM)
        self.soc.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1) # <3
        self.soc.bind(("", PORT))
        self.alive = True
        self.connections = []

    def waitForClients(self, n):
        """ Block until n clients are connected """
        while len(self.connections)<n: sleep(0.1)

    def run(self):
        """
        When a client tries to connect, the server adds him to the list and
        creates a new thread managing communications with this client.
        """
        self.soc.listen(1024) # ça ne borne pas la taille de la liste ?
        while self.alive:
            soc, addr = self.soc.accept()
            print(addr, "connected")
            co = ServerConnection(soc, self.handle, self)
            self.connections.append(co)
            co.start()

    def sendOrder(self, ident, order):
        """
        Send an order to all connected clients by broadcasting messages to
        all threads in the list.
        """
        self.broadcast(bytes((ident//256, ident%256)) + order.toBytes())

    #def send(self, m, dest): self.soc.sendto(m, (HOST, PORT))

    def broadcast(self, m):
        for co in self.connections:
            co.send(m)

    def kill(self): self.alive = False

