"""
This module handles the server of the game.
The server runs the main loop and receives the request of actions
from the client. If the actions are allowed by the description of the game
in the xml files, all clients are asked to execute the actions.
"""

from sys import stdin
from argparse import ArgumentParser
from functools import partial
import asyncio

from shared.const import PATH
from shared.orders import OrderDispatcher
from shared.tools import Timer
from shared.network import NetworkServer
import shared.world as world
from management.actions import registerActions
from management.console import welcomeMessage, inputReady
from parsing.global_parsing import game_parser
from plugins.plugin import loadPluginsServer


class Server():
    """ Main class of the server process, gathering network, world, actions and timer """

    def __init__(self, path):
        self.loop = asyncio.get_event_loop()
        self.net = NetworkServer(self.handleEvent, self.pluginHandle, self.loop)
        parseData = game_parser(path)
        self.world = world.loadGame(parseData)
        self.actions = registerActions(parseData['Actions'], world.named) # FIXME -> game
        self.plugins = loadPluginsServer(parseData['Plugins'], self)

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
        welcomeMessage(SERVER)
        await self.net.waitForClients(1)#len(self.world.entities))
        await self.handleEvent(self.world, "start")
        while True:
            emitter, event = await self.events.get()
            if event == "end":
                break # TODO gérer la fin
            if event == "pause":
                self.orderDispatcher.timer.pause = True
                self.pause = True
            if self.pause and event == "resume":
                self.orderDispatcher.timer.pause = False
                self.pause = False
                continue
            if self.pause:
                continue
            if event not in self.actions:
                continue
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
                plug.serverMessage(msg[len(plug.MSGID):])

PARSER = ArgumentParser(description="Generic game server.")
PARSER.add_argument("-p", "--path", default=PATH,
                    help="Path of the game directory, should contain game.xml.\
                         If this argument is not present, const.py will be used.")
ARGS = PARSER.parse_args()
SERVER = Server(ARGS.path+"/")

try:
    SERVER.run()
except KeyboardInterrupt:
    pass
