"""
    This module only define the class CursesPlugin,
    which is a class to write a curse plugin
"""
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

    def __init__(self, logic):#, interface):
        self.win = None # sera chargée quand on aura tous les plugins
        self.logic = logic
        self.interface = None

    def handleKey(self, key):
        """ return True if the key has been used """
        return False

    def draw(self):
        """ don't call self.win.noutrefresh, it will be done when needed """
        self.height, self.width = self.win.getmaxyx() # XXX bof bof

    def repaint(self):
        """ update display """
        self.draw()
        curses.doupdate()
