from threading import Thread
from socket import socket, SOCK_STREAM, AF_INET, AF_INET6, SOL_SOCKET, SO_REUSEADDR
from time import sleep

from const import *
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

    def run(self):
        """
        Main loop of the client's network thread.  After connecting the socket
        to the server, wait for orders and handle them immediately until
        connection ends.
        """
        self.soc.connect((HOST, PORT))
        while self.alive:
            msg = self.soc.recv(BUFF)
            #print("recieved: " + str(msg))
            if not msg:
                print("Connection lost\n")
                self.alive = False
            size = 0
            while size < len(msg):
                ident = msg[size]*256 + msg[size+1]
                order, i = Order().fromBytes(msg[size+2:])
                if self.alive:
                    self.handle(ident, order)
                #print(msg[size+2:size+2+i])
                size += i + 2

    def send(self, m):
        self.soc.sendall(m)

    def sendEvent(self, obj, eve):
        """
        Send an event to the server in a formatted way, specifying the id
        of the object to affect.
        """
        m = bytes([obj.ident//256, obj.ident%256]) + bytes(eve, CODING)
        self.send(m)

    def kill(self):
        self.soc.close()
        self.alive = False

class ServerConnection(Thread):
    """
    This thread manages the communications with one particular client (one
    thread is created by client).
    """
    def __init__(self, soc, han):
        Thread.__init__(self)
        self.soc = soc
        self.handle = han

    def run(self):
        """
        Wait for messages from the client and handle them immediately.
        """
        while True:
            # !! plusieurs events peuvent être contenus dans un msg (TCP)
            msg = self.soc.recv(BUFF) # + gestion d'erreurs
            ident = msg[0]*256 + msg[1]
            event = msg[2:].decode(CODING)
            self.handle(ident, event)

    def send(self, m):
        self.soc.sendall(m)

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
        self.co = []

    def waitForClients(self, n):
        """ ? """
        while len(self.co)<n: sleep(0.1)

    def run(self):
        """
        When a client tries to connect, the server adds him to the list and
        creates a new thread managing communications with this client.
        """
        self.soc.listen(1024) # ça ne borne pas la taille de la liste ?
        while self.alive:
            soc, addr = self.soc.accept()
            print(addr, "connected")
            co = ServerConnection(soc, self.handle)
            self.co.append(co)
            co.start()

    def sendOrder(self, ident, order):
        """
        Send an order to all connected clients by broadcasting messages to
        all threads in the list.
        """
        self.broadcast(bytes((ident//256, ident%256)) + order.toBytes())

    #def send(self, m, dest): self.soc.sendto(m, (HOST, PORT))

    def broadcast(self, m):
        for co in self.co:
            co.send(m)

    def kill(self): self.alive = False

