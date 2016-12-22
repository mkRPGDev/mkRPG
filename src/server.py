from sys import stdin
from argparse import ArgumentParser
from functools import partial
import asyncio

from const import PATH
from shared.orders import OrderDispatcher
from shared.tools import Perf, Timer
from shared.network import NetworkServer
from serverside.actions import registerActions
from serverside.console import welcomeMessage, inputReady
from parsing.global_parsing import game_parser
from plugins.plugin import loadPluginsServer

import shared.world as world

class Server():
    """ Main class of the server process, gathering network, world, actions and timer """

    def __init__(self, path):
        self.loop = asyncio.get_event_loop()
        self.net = NetworkServer(self.handleEvent, self.pluginHandle, self.loop)
        parseData = game_parser(path)
        self.world = world.loadGame(parseData)
        self.actions = registerActions(parseData['Actions'], world.named) # FIXME -> game
        self.plugins = loadPluginsServer(path, self)

        self.timer = Timer()
        self.orderDispatcher = OrderDispatcher(self.world, self.handleEvent, self.timer)
        self.events = asyncio.Queue()
        self.pause = False
        
    def __del__(self):
        self.loop.stop()
#        print("Killing server")

    def run(self):
        """ Launch tasks """
        self.loop.create_task(self.timer.run())
        self.loop.create_task(self.net.run())
        self.loop.add_reader(stdin, partial(inputReady, self))
        self.loop.run_until_complete(self.main())

    async def main(self):
        """ Init stuff, read events and ask for treatment """
        welcomeMessage(server)
        await self.net.waitForClients(len(self.world.entities))
        await self.handleEvent(self.world, "start")
        while True:
            emitter, event = await self.events.get()
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
        """ Add a event to the queue """
        await self.events.put((emitter, event))

    async def pluginHandle(self, msg):
        """ Search for plugins that want to handle the message """
        for plug in self.plugins:
            if msg.startswith(plug.MSGID):
                await plug.serverMessage(msg)

parser = ArgumentParser(description="Generic game server.")
parser.add_argument("-p", "--path", default=PATH,
    help="Path of the game directory, should contain game.xml."
    "If this argument is not present, const.py will be used.")
args = parser.parse_args()
server = Server(args.path+"/")

try:
    server.run()
except KeyboardInterrupt:
    pass
