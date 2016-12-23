from argparse import ArgumentParser
import asyncio

from interface.interactions import registerInteractions, InteractionType
from interface.interface import skeys
from const import UPDTIME, PATH
from shared.orders import OrderDispatcher
from shared.network import NetworkClient
from parsing.global_parsing import game_parser
from plugins.plugin import loadPluginsClient

import shared.world as world
from shared.tools import Perf

perf=Perf()

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
        parseData = game_parser(path)
        self.world = world.loadGame(parseData)
        plugins = loadPluginsClient(parseData["Plugins"], self, args.curses, args.pygame)
        self.interface = Interface(self.world, parseData["Images"], plugins.graphical)
        if args.debug: exit(0)
        self.plugins = plugins.logical
        self.interactions = registerInteractions(parseData["Interactions"])
        self.perso = None
        self.orderDispatcher = OrderDispatcher(self.world, None, None)
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
        self.netTask = self.loop.create_task(self.net.run())
        self.loop.run_until_complete(self.main())

    async def getEntity(self):
        """ Ask for a free entity to the server.
        May disappear with a proper initialisation process """
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
            # XXX désolé je ne supporte pas d'entendre mon ordi souffler pour rien
            perf.tic()
            keys = self.interface.getEvent()
            perf.toc()
            if not keys:
                await asyncio.sleep(UPDTIME)
            else:
                await asyncio.sleep(0)
            for key in keys:
                if key==skeys.QUIT:
                    return
                elif key==skeys.PAUSE: await self.net.sendEvent(self.world, "pause")
                # TODO relayer la pause à l'affichage
                elif key==skeys.RESUME: await self.net.sendEvent(self.world, "resume")
                for inte in self.interactions:
                    if (inte.type == InteractionType.Key and
                        inte.key == key):
                        await self.net.sendEvent(self.__getattribute__(inte.target),
                                                 inte.event)

    async def handleOrder(self, ident, order):
        """ Call dispatcher and update display """
        emitter = world.Object.ids[ident]
        await self.orderDispatcher.treat(emitter, order)
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

cli = Client(args.path+"/", interface(args))
try:
    cli.run()
except KeyboardInterrupt:
    pass
except:
    cli.interface.end()
    raise
finally:
    perf.show()

