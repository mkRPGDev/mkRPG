"""
This file describes a ncurses chat for the game.
This chat can be used when using the terminal mode of the game.
"""

import curses
from plugins.plugincurses import CursesPlugin


class ChatView(CursesPlugin):
    """
    This describes a view from the chat, to be displayed on the screen
    at runtime.
    """
    MINW = 1000
    MINH = 9
    X = 0
    Y = -MINH
    NAME = b"Chat"
    HELP = b"space : begin message", b"enter : send message"

    def __init__(self, *args):
        self.curses = curses # Pas top
        super().__init__(*args)
        self.writing = False
        self.text = bytearray()

    def reset(self):
        """ Resets the object attributes. """
        self.writing = False
        self.text = bytearray()

    def handleKey(self, key):
        """ return True if the key has been used """
        if key == ord(' ') and not self.writing:
            self.writing = True
            return True
        if not self.writing:
            return False
        elif key == curses.KEY_BACKSPACE:
            self.text.pop() if len(self.text) else self.reset()
        elif key==ord('\n'):#curses.KEY_ENTER:
            self.logic.send(self.text)
            self.reset()
        elif key == curses.KEY_EXIT:
            self.reset()
        elif key in range(32, 127):
            self.text.append(key)
        else:
            return False
        return True

    def draw(self):
        """ Draws the chat on the screen and the texts. """
        super().draw()
        self.win.erase()
        self.win.border()

        # Older messages
        for k in range(1, self.height-2):
            if len(self.netPlugin.msgs) < k:
                break
            self.win.addstr(self.height-k-2, 1, self.netPlugin.msgs[-k])
        self.win.addch(1, self.width-2, curses.ACS_UARROW)
        self.win.addch(self.height-3, self.width-2, curses.ACS_DARROW)
        
        # Zone d'entrée
        text = self.text[max(0, len(self.text)-self.width+10):]
        self.win.addstr(self.height-2, 1, bytes(text))
        if self.writing:
            self.win.addch(self.height-2, len(text)+1, ord('|'))
        else:
            self.win.addch(self.height-2, len(text)+1, ord('_'))
