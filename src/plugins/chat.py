from plugins.plugin import Plugin, CursesPlugin

class Chat(Plugin):
    MSGID = b"chat"

    def __init__(self, e):
        super().__init__(e)
        self.msgs = ["Connecté"]
        
    async def serverMessage(self, msg):
        await self.engine.net.broadcast(bytes([0,0,len(msg)//256, len(msg)%256])+msg)
    
    async def clientMessage(self, msg):
        self.msgs.append(msg)
        if self.ui: self.ui.repaint()
    
    def serverCallback(self):
        pass

import curses

class ChatUI(CursesPlugin):
    MINW = 1000
    MINH = 9
    X = 0
    Y = -MINH
    NAME = b"Chat"
    HELP = b"space : begin message", b"enter : send message"
    
    def __init__(self, *args):
        super().__init__(*args)
        self.writing = False
        self.text = bytearray()
    
    def reset(self):
        self.writing = False 
        self.text = bytearray()
        
    def handleKey(self, key):
        """ return True if the key has been used """
        if key==ord(' ') and not self.writing:
            self.writing = True
            return True
        if not self.writing: return False
        elif key==curses.KEY_BACKSPACE:
            self.text.pop() if len(self.text) else self.reset()
        elif key==ord('\n'):#curses.KEY_ENTER:
            self.netPlugin.send(self.text)
            self.reset()
        elif key==curses.KEY_EXIT:
            self.reset()
        elif key in range(32, 127):# or key in range(97,123) or key in range(65,91) or key in range(48,58):
            self.text.append(key)
        else:
            return False
        return True
    
    def draw(self):
        super().draw()
        self.win.erase()
        self.win.border()
        # Anciens messages
        for k in range(1,self.height-2):
            if len(self.netPlugin.msgs)<k: break
            self.win.addstr(self.height-k-2,1,self.netPlugin.msgs[-k])
        self.win.addch(1,self.width-2,curses.ACS_UARROW)
        self.win.addch(self.height-3,self.width-2,curses.ACS_DARROW)
        # Zone d'entrée
        text = self.text[max(0, len(self.text)-self.width+10):]
        self.win.addstr(self.height-2,1,bytes(text))
        if self.writing:
            self.win.addch(self.height-2,len(text)+1,ord('|'))
        else:
            self.win.addch(self.height-2,len(text)+1,ord('_'))
