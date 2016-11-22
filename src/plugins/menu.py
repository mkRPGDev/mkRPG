from plugins.plugin import CursesPlugin

import curses

class MenuView(CursesPlugin):
    """ manages the curses menu windows """
    MINW = 26
    MINH = 1000
    X = -MINW
    Y = 0
    NAME = "Menu"

    def draw(self):
        super().draw()
        self.win.border()
        self.centered(3, "MKRPG", curses.A_BOLD)    
        self.centered(5, "The curses client", curses.A_BOLD)
        self.win.addstr(9,  2, b"[r] : start/resume")
        self.win.addstr(10, 2, b"[p] : pause")
        self.win.addstr(12, 2, b"[w] : quit")
        self.win.addstr(14,  2, b"arrows : move map view")
        self.win.addstr(16,  2, b"[l] : show/hide")
        self.win.addstr(17,  2, b"      lignes of view")
        self.win.addstr(18,  2, b"[f] : follow me")
        y = 20
        for p in self.interface.plugins:
            if not p.HELP: continue
            self.win.addstr(y,  2, p.NAME+b" plugin", curses.A_BOLD)
            for h in p.HELP:
                y+=1
                self.win.addstr(y,  2, h)
            y+=2
        self.centered(self.height-2, "the mkrpg team")
        self.win.noutrefresh()
    
    def centered(self, y, st, attr = None):
        """ same as addstr but centered """
        if len(st)>self.width: return
        if attr:
            self.win.addstr(y, (self.width-len(st))//2, st, attr)
        else: 
            self.win.addstr(y, (self.width-len(st))//2, st)

