from asyncio import ensure_future

class Plugin:
    """ To write a network plugin, subclass this and reimplement attributes
    and virtual methods"""
    CBTIME = None
    MSGID = b""
    
    def __init__(self, engine):
        self.engine = engine
        self.ui = None
        
    async def serverMessage(self, msg):
        """ called then bytes msg are sent to the server """
        pass
    
    async def clientMessage(self, msg):
        """ called then bytes msg are sent to the client """
        pass
    
    def send(self, msg):
        l = len(self.MSGID)+len(msg)
        ensure_future(self.engine.net.send(bytes([0,0,l//256,l%256])+self.MSGID+msg))
    
    def serverCallback(self):
        pass

import curses

class CursesPlugin:
    """ Subclass this to write a curse plugin, appearing as a window.
    MINW and MINH are the asked size and X,Y the placement that can be
    negative to be on the other side.
    Fill NAME and HELP to have an entry in the menu. """
    MINW = 0
    MINH = 0
    NAME = ""
    HELP = ()
    
    def __init__(self, netPlugin):#, interface):
        self.win = None # sera chargée quand on aura tous les plugins
        self.netPlugin = netPlugin
        self.interface = None
        
    def handleKey(self, key):
        """ return True if the key has been used """
        return False
    
    def draw(self):
        """ don't call self.win.noutrefresh, it will be done when needed """
        self.height, self.width = self.win.getmaxyx() # XXX bof bof

    def repaint(self):
        self.draw()
        curses.doupdate()

class PygamePlugin: 
    """ To be defined by a PyGame Pro... """    
    pass


def loadPluginsServer(path, engine):
    # FIXME gros bricolage
    import plugins.chat as chat
    ch = chat.Chat(engine)
    return [ch]

def loadPluginsClient(path, engine):
    import plugins.chat as chat
    import plugins.menu as menu
    ch = chat.Chat(engine)
    chc = chat.ChatUI(ch)#, engine.interface)
    ch.ui = chc
    men = menu.MenuView(None)#, engine.interface)
    return [ch],[men, chc]

