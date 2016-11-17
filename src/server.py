from sys import argv
from queue import Queue
from argparse import ArgumentParser
import asyncio

from const import *
from actions import registerActions
from orders import OrderDispatcher
<<<<<<< HEAD
from tools import Perf
=======
from network import NetworkServer
from utils import Timer
>>>>>>> server

import world

class Server():
    """ Classe principale du processus serveur, concilie réseau, monde, actions et timer """

    def __init__(self, path):
        self.loop = asyncio.get_event_loop()
        self.net = NetworkServer(self.handleEvent, self.loop)
        self.world = world.loadGame(path)
        self.actions = registerActions(path, world.named) # FIXME -> game

        self.timer = Timer()
        self.orderDispatcher = OrderDispatcher(self.world, self.handleEvent, self.timer)
        self.events = asyncio.Queue()
        self.pause = False
        self.coEntities = {}

    def __del__(self):
        self.loop.stop()
        print("Killing server")

    def run(self):
        self.loop.create_task(self.timer.run())
        self.loop.create_task(self.net.run())
        self.loop.run_until_complete(self.main())

    async def main(self):
        print("Server started; waiting for",len(self.world.entities),"clients")
        await self.net.waitForClients(len(self.world.entities))
        await self.handleEvent(self.world, "start")
        while True:
            emitter, event = await self.events.get()
            #print("ev", event)
            if event == "end": break # TODO gérer la fin
            if event == "pause":
                self.orderDispatcher.timer.pause = True
                self.pause = True
            if self.pause and event == "resume":
                self.orderDispatcher.timer.pause = False
                self.pause = False
                continue
            if self.pause: continue
            if event not in self.actions: continue
            for act in self.actions[event]:
                for order in act.orders:
                    returnOrder = await self.orderDispatcher.treat(emitter, order)
                    if returnOrder:
                        await self.net.sendOrder(emitter.ident, returnOrder)
                    # TODO n'envoyer que les infos non secrètes
        
    async def handleEvent(self, emitter, event):
        await self.events.put((emitter, event))

parser = ArgumentParser(description="Generic game server.")
parser.add_argument("-p", "--path", default=PATH,
    help="Path of the game directory, should contain game.xml."
    "If this argument is not present, const.py will be used.")
args = parser.parse_args()
server = Server(args.path)

try:
    server.run()
except KeyboardInterrupt:
    pass
