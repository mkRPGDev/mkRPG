from time import sleep, time
from argparse import ArgumentParser
import asyncio

from const import *
from interface.interactions import registerInteractions, InteractionType
from interface.interface import skeys
from shared.orders import OrderDispatcher
from shared.network import NetworkClient
import shared.world as world
from plugins.plugin import loadPluginsClient


def interface(args):
    """ Import the correct interface according to user choice """
    if args.curses:
        from interface.cursescli import Curses as Interface
    elif args.noui:
        from interface.interface import Interface
    elif args.pygame:
        from interface.pygamecli import Client as Interface
    else:
        # choix par défaut
        args.curses = True
        return interface(args)
    return Interface

class Client():
    """ Main class of the client process, gathering interface, world and networking"""
    def __init__(self, path, Interface):
        self.loop = asyncio.get_event_loop()
        self.net = NetworkClient(self.handleOrder, self.pluginHandle)
        self.world = world.loadGame(path)
        self.plugins = loadPluginsClient(path, self)
        # TODO intégrer au loadGame, faire une autre classe client ?
        self.interface = Interface(self.world, self.plugins[1])
        self.plugins = self.plugins[0]
        self.interactions = registerInteractions(path)
        self.perso = None
        self.orderDispatcher = OrderDispatcher(self.world, None, None)

    def __del__(self):
        """ Kill network and interface """
        self.net.kill()
        self.loop.stop()
        self.interface.end()
        print("Client killed")

    def run(self):
        self.loop.run_until_complete(self.net.connect())
        self.loop.run_until_complete(self.getEntity())
        self.netTask = self.loop.create_task(self.net.run())
        self.loop.run_until_complete(self.main())

    async def getEntity(self):
        for ent in self.world.entities:
            if await self.net.askEntity(ent):
                self.perso = ent
                self.interface.setPerso(self.perso)
                break
        else:
            print("No available entity.")
            return

    async def main(self):
        self.interface.init()
        while True:
#            self.interface.update()
            # XXX désolé je ne supporte pas d'entendre mon ordi souffler pour rien
            keys = self.interface.getEvent()
            if not keys:
                await asyncio.sleep(UPDTIME)
            for key in keys:
                if key==skeys.QUIT:
                    return
                elif key==skeys.PAUSE: await self.net.sendEvent(self.world, "pause")
                # TODO relayer la pause à l'affichage
                elif key==skeys.RESUME: await self.net.sendEvent(self.world, "resume")
                for inte in self.interactions:
                    if (inte.type == InteractionType.Key and
                        inte.key == key):
                        await self.net.sendEvent(self.__getattribute__(inte.target), inte.event)

    async def handleOrder(self, ident, order):
        emitter = world.BaseObject.ids[ident]
        await self.orderDispatcher.treat(emitter, order)
        self.interface.update()

    async def pluginHandle(self, msg):
        for plug in self.plugins:
            if msg.startswith(plug.MSGID):
                await plug.clientMessage(msg[len(plug.MSGID):])


parser = ArgumentParser(description="Generic game client.")
parser.add_argument("-p", "--path", default=PATH,
                    help="Path of the game directory, should contain game.xml."
                    "If this argument is not present, const.py will be used.")

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

if args.debug: asyncio.get_event_loop().set_debug(True)

cli = Client(args.path, interface(args))
try:
    cli.run()
except KeyboardInterrupt:
    pass
except:
    cli.interface.end()
    raise
