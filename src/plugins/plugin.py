from asyncio import ensure_future
from collections import namedtuple
from importlib import import_module

from const import IDLEN

class Plugin:
    """ To write a network plugin, subclass this and reimplement attributes
    and virtual methods"""
    CBTIME = None
    MSGID = b""

    def __init__(self, engine):
        self.engine = engine
        self.ui = None

    def serverMessage(self, msg):
        """ called then bytes msg are sent to the server """
        pass

    def clientMessage(self, msg):
        """ called then bytes msg are sent to the client """
        pass

    def send(self, msg):
        l = len(self.MSGID)+len(msg)
        ensure_future(self.engine.net.send(b"\x00"*IDLEN+l.to_bytes(2,'big')+self.MSGID+msg))

    def broadcast(self, msg):
        msg = self.MSGID + msg
        ensure_future(self.engine.net.broadcast(b"\x00"*IDLEN+len(msg).to_bytes(2,'big')+msg))

    def serverCallback(self):
        pass

def loadPluginsServer(names, engine):
    """ Load serverside plugins """
    plugins = []
    for name in names:
        try:
            module = import_module("plugins."+name)
        except ImportError: continue
        # TODO si vous avez mieux que eval...
        try:
            plugins.append(eval('module.'+name.capitalize())(engine))
        except:
            print("Error loading plugin",name)
    return plugins

def loadPluginsClient(names, engine, curses, pygame):
    """ Load clientside plugins with the suitable UI class """
    plugins, uiplugins = [], []
    for name in names:
        try:
            module = import_module("plugins."+name)
            logic = eval('module.'+name.capitalize())(engine)
        except ImportError:
            logic = None
        ui = None
        if curses:
            try:
                module = import_module("plugins."+name+"curses")
                ui = eval('module.'+name.capitalize()+"View")(logic)
            except ImportError: pass
        elif pygame:
            try:
                module = import_module("plugins."+name+"pygame")
                ui = eval('module.'+name.capitalize()+"Surface")(logic)
            except ImportError: pass
        if logic: plugins.append(logic)
        if ui:
            uiplugins.append(ui)
            if logic: logic.ui = ui
    return namedtuple("PluginType", "logical graphical")(plugins, uiplugins)

