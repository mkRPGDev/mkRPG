from time import sleep
import asyncio

from shared.const import HOST, PORT, BUFF, IDLEN, CODING
from shared.world import Object, retrieveWorld
from shared.orders import Order # XXX à sa place ?

PRINTDEBUG = False

class NetworkClient:
    """
    This class manages the network activities of the client.
    It allows the client to send messages (describing events) to the server.
    """

    def __init__(self, handle, pluginHandle):
        self.handle = handle
        self.pluginHandle = pluginHandle
#        self.soc = socket(AF_INET6 if IPV6 else AF_INET, SOCK_STREAM)

    async def askEntity(self, ent):
        """ Ask for an entity """
        await self.sendEvent(ent, "acquire")
        msg = await self.reader.read(BUFF) # TODO plusieurs messages
        return msg == b"accepted"

    async def connect(self):
        self.reader, self.writer = await asyncio.open_connection(HOST, PORT)

    async def run(self):
        """
        Main loop of the client's network task.  After connecting the socket
        to the server, wait for orders and handle them immediately until
        connection ends.
        """

        while True:
            msg = await self.reader.read(BUFF)
            if len(msg)==BUFF:
                raise RuntimeError("Network flooded, consider increasing BUFF")
            size = 0
            if not msg: return # TODO remonter l'info et l'afficher
            while size < len(msg):
                ident = int.from_bytes(msg[size:size+IDLEN], 'big')
                if ident == 0:
                    length = int.from_bytes(msg[size+IDLEN:size+IDLEN+2], 'big')
                    await self.pluginHandle(msg[size+IDLEN+2:size+IDLEN+2+length])
                    length += 2
                else:
                    order, length = Order().fromBytes(msg[size+IDLEN:])
                    await self.handle(ident, order) # TODO casser l'asymétrie
                size += length + IDLEN

    async def send(self, m):
        self.writer.write(m)
        if PRINTDEBUG: print("Sent %s" % str(m)[1:])
        await self.writer.drain()

    async def sendEvent(self, obj, event):
        """
        Send an event to the server in a formatted way, specifying the id
        of the object to affect.
        """
        assert(len(event)<256)
        m = obj.ident.to_bytes(IDLEN, 'big') + bytes([len(event)]) + \
            bytes(event, CODING)
        await self.send(m)

    def kill(self):
        self.writer.close()

class ServerConnection:
    """
    This class manages the communications with one particular client (one
    task is created by client).
    """
    def __init__(self, reader, writer, handle, pluginHandle, parent):
        self.reader = reader
        self.writer = writer
#        self.soc.settimeout(1)
        self.handle = handle
        self.pluginHandle = pluginHandle
        self.entity = None
        self.server = parent

    async def run(self):
        """
        Wait for messages from the client and handle them immediately.
        """
        while True:
            try:
                msg = await self.reader.read(BUFF)
            except ConnectionResetError:
                self.end()
                return
            if not msg: return
            while msg:
                ident = int.from_bytes(msg[:IDLEN], 'big')
                if ident == 0:
                    length = int.from_bytes(msg[IDLEN:IDLEN+2], 'big')
                    await self.pluginHandle(msg[IDLEN+2:IDLEN+2+length])
                    length += 2
                else:
                    length = msg[IDLEN]
                    event = msg[IDLEN+1:IDLEN+1+length].decode(CODING)
                    assert ident in Object.ids
                    emitter = Object.ids[ident]
                    if self.entity: # TODO and self.entity.ident == ident:
                        await self.handle(emitter, event)
                    else:
                        assert event == "acquire"
                        if not emitter.user:
                            emitter.user = self
                            self.entity = emitter
                            await self.handle(emitter, "init")
                            await self.send(b"accepted")
                            for ident, order in retrieveWorld():
                                asyncio.run_coroutine_threadsafe(
                                    self.send(ident.to_bytes(IDLEN, 'big') + 
                                    order.toBytes()), asyncio.get_event_loop())
                                # on effectue tout le transfert avant de continuer
                        else:
                            await self.send(b"rejected")
                msg = msg[IDLEN+1+length:]

    async def send(self, m):
        try:
            self.writer.write(m)
            if PRINTDEBUG: print("Sent %s" % str(m)[1:])
            await self.writer.drain()
        except ConnectionResetError:
            self.end()

    def end(self):
        if not self.server: return
#        print("Déco")
        if self.entity: self.entity.user = None
        self.writer.close()
        self.server.connections.remove(self)
        self.server = None


class NetworkServer:
    """
    This class is the task of the server that manages clients connections on
    his port.  It keeps the list of connected clients and provides method to
    broadcast messages to all the clients.
    """

    def __init__(self, handle, pluginHandle, loop):
        """ The server listen on the port specified in const.py. """
        self.handle = handle
        self.pluginHandle = pluginHandle
        self.loop = loop
        self.connections = []

    async def waitForClients(self, n):
        """ Block until n clients are connected """
        while len(self.connections)<n: await asyncio.sleep(0.1)

    async def connect(self, reader, writer):
        co = ServerConnection(reader, writer, self.handle, self.pluginHandle, self)
        self.loop.create_task(co.run())
        self.connections.append(co)

    async def run(self):
        """
        When a client tries to connect, the server adds him to the list and
        creates a new task managing communications with this client.
        """
        self.server = await asyncio.start_server(self.connect, port=PORT, host="",
                                                 reuse_address=1, loop=self.loop)

    async def sendOrder(self, ident, order):
        """
        Send an order to all connected clients by broadcasting messages to
        all threads in the list.
        """
        await self.broadcast(ident.to_bytes(IDLEN, 'big') + order.toBytes())

    #def send(self, m, dest): self.soc.sendto(m, (HOST, PORT))

    async def broadcast(self, m):
        for co in self.connections:
            await co.send(m)


# Prémices de tests

#import network
#import asyncio
#loop = asyncio.get_event_loop()
#ns = network.NetworkServer(lambda a,b:print(5), loop)
#loop.create_task(ns.run())

#async def main()
#    pass
#
#loop.run_until_complete(main())

###################

#import network
#import asyncio
#loop = asyncio.get_event_loop()
#nc = network.NetworkClient(lambda a,b:print(5))
#loop.create_task(nc.run())

#async def main()
#    pass
#
#loop.run_until_complete(main())
