# -*- coding: utf-8 -*-

"""
    This module contains the main Client class.
"""

from argparse import ArgumentParser
import asyncio

from interface.interactions import registerInteractions, InteractionType
from interface.interface import skeys
from shared.const import UPDTIME, PATH
from shared.orders import OrderDispatcher
from shared.network import NetworkClient
from parsing.global_parsing import game_parser
from plugins.plugin import loadPluginsClient

import shared.world as world
from shared.tools import Perf

PERF = Perf()

def interface(args):
    """ Import the correct interface according to user choice """
    if args.noui:
        from interface.interface import Interface
    elif args.curses:
        from interface.cursescli import Curses as Interface
    elif args.pygame:
        from interface.pgcli import Pygame as Interface
    else:
        # choix par défaut
        args.pygame = True
        return interface(args)
    return Interface

class Client:
    """ Main class of the client process, gathering interface, world and networking """

    def __init__(self, path, Interface):
        self.loop = asyncio.get_event_loop()
        self.net = NetworkClient(self.handleOrder, self.pluginHandle)
        parse_data = game_parser(path)
        self.world = world.loadGame(parse_data)
        plugins = loadPluginsClient(parse_data["Plugins"], self, args.curses, args.pygame)
        self.interface = Interface(self.world, parse_data["Images"], plugins.graphical)
        if args.debug:
            exit(0)
        self.plugins = plugins.logical
        self.interactions = registerInteractions(parse_data["Interactions"])
        self.perso = None
        self.order_dispatcher = OrderDispatcher(self.world, None, None)
        self.task = None

    def __del__(self):
        """ Kill network and interface """
        self.net.kill()
        self.loop.stop()
        self.interface.end()
        print("Client killed")

    def run(self):
        """ Launch tasks """
        self.loop.run_until_complete(self.net.connect())
        self.loop.run_until_complete(self.getEntity())
        self.net_task = self.loop.create_task(self.net.run())
        self.loop.run_until_complete(self.main())

    async def getEntity(self):
        """
            Asks for a free entity to the server and sets the client and
            interface attributes.
            If either args.entity or args.entityid is set, will ask for this
            entity to the server and set it as the client entity, else it will
            ask for the first available entity.
        """
        if (args.entityid, args.entity) != (None, None):
            ent = args.entityid if args.entityid is not None \
                else self.world.named[args.entity]
            if ent in self.world.entities and await self.net.askEntity(ent):
                self.perso = ent
                self.interface.setPerso(self.perso)
            else:
                print("Specified entity is not available.")
                return
        else:
            for ent in self.world.entities:
                if await self.net.askEntity(ent):
                    self.perso = ent
                    self.interface.setPerso(self.perso)
                    break
            else:
                print("No available entity.")
                return

    async def main(self):
        """ Init stuff, listen for inputs and send corresponding events """
        self.interface.init()
        while True:
            self.interface.update()
            PERF.tic()
            keys = self.interface.getEvent()
            PERF.toc()
            if not keys:
                await asyncio.sleep(UPDTIME)
            else:
                await asyncio.sleep(0)
            for key in keys:
                if key == skeys.QUIT:
                    self.__del__()
                    return
                elif key == skeys.PAUSE:
                    await self.net.sendEvent(self.world, "pause")
                # TODO relayer la pause à l'affichage
                elif key == skeys.RESUME:
                    await self.net.sendEvent(self.world, "resume")
                for inte in self.interactions:
                    if inte.type == InteractionType.Key and inte.key == key:
                        await self.net.sendEvent(self.__getattribute__(inte.target),
                                                 inte.event)

    async def handleOrder(self, ident, order):
        """ Call dispatcher and update display """
        if ident in world.Object.ids:
            emitter = world.Object.ids[ident]
        else: # si on ne peut convertir c'est que l'objet va être 'Create'd
            emitter = ident
        await self.order_dispatcher.treat(emitter, order)
        self.interface.update()

    async def pluginHandle(self, msg):
        """ Search for plugins that want to handle the message """
        for plug in self.plugins:
            if msg.startswith(plug.MSGID):
                plug.clientMessage(msg[len(plug.MSGID):])


parser = ArgumentParser(description="Generic game client.")
parser.add_argument("-p", "--path", default=PATH,
                    help="Path of the game directory, should contain game.xml."
                    "If this argument is not present, const.py will be used.")

# Fusionner les deux et tester si la chaine contient un int ?
# -> Impossible de donner des noms d'entity de type "1234"
entgroup = parser.add_mutually_exclusive_group()
entgroup.add_argument("-e", "--entity", action="store",
                      help="Entity name to be used by the client.")
entgroup.add_argument("-eid", "--entityid", type=int, action="store",
                      help="Entity id to be used by the client.")

uimode = parser.add_mutually_exclusive_group()
uimode.add_argument("-c", "--curses", action="store_true",
                    help="Use ncurses (requires Unix) instead of PyGame.")
uimode.add_argument("-g", "--pygame", action="store_true",
                    help="Use PyGame interface. (default)")
uimode.add_argument("-n", "--noui", action="store_true",
                    help="Hide UI for debug purposes")

parser.add_argument("-d", "--debug", action="store_true",
                    help="Activate asyncio debug mode.")

args = parser.parse_args()

if args.debug:
    asyncio.get_event_loop().set_debug(True)

cli = Client(args.path+"/", interface(args))
try:
    cli.run()
except KeyboardInterrupt:
    pass
except:
    cli.interface.end()
    raise
finally:
    PERF.show()
