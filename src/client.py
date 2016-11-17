from time import sleep, time
from argparse import ArgumentParser
import asyncio

from const import *
from interactions import registerInteractions, InteractionType
from orders import OrderDispatcher
from cache import ChunkCache

from network import NetworkClient
import world
from printWorld import WorldViewer, Interface
from utils import add_to_rect_list

from pygame.locals import K_ESCAPE, K_p, K_r

def interface(args):
    """ Import the correct interface according to user choice """
    if args.curses:
        from cursescli import Curses as Interface
    elif args.noui:
        from interface import Interface
    elif args.pygame:
        from pygamecli import Client as Interface
    else:
        # choix par défaut
        args.curses = True
        return interface(args)
    return Interface

class Client():
    """ Main class of the client process, gathering interface, world and networking"""
    def __init__(self, path, Interface):
        self.loop = asyncio.get_event_loop()
        self.net = NetworkClient(self.handleOrder)
        self.world = world.loadGame(path)
        self.interface = Interface(self.world)
        self.interactions = registerInteractions(path)
        self.perso = None
        self.orderDispatcher = OrderDispatcher(self.world, None, None)
#        print(self.world.entities)

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
                self.interface.set_perso(self.perso)
                break
        else:
            print("No available entity.")
            return

    async def main(self):
        self.interface.init()
        while True:
            await asyncio.sleep(0)
            self.interface.update()
            keys = self.interface.getEvent()
            for key in keys:
                if key==K_ESCAPE:
                    del self.interface
                    del self
                    return
                elif key==K_p: await self.net.sendEvent(self.world, "pause")
                # TODO relayer la pause à l'affichage
                elif key==K_r: await self.net.sendEvent(self.world, "resume")
                for inte in self.interactions:
                    if (inte.type == InteractionType.Key and
                        inte.key == key):
                        await self.net.sendEvent(self.__getattribute__(inte.target), inte.event)

    async def handleOrder(self, ident, order):
        emitter = world.BaseObject.ids[ident]
        await self.orderDispatcher.treat(emitter, order)

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
finally:
    cli.interface.end()
