from interface import Interface

import curses

class Curses(Interface):
    """ ncurses-based UI """
    def __init__(self, w):
        super().__init__(w)
        self.win = curses.initscr()
        curses.cbreak()
        curses.noecho()
        self.win.keypad(True)
        curses.curs_set(0)
        self.mv=MapViewer(self.world.currentMap, self.world)

    def repaint(self):
        self.mv.display(self.win)
        # TODO insérer ici le xml d'interface
        self.win.addstr(26,0,"Score "+str(self.world.entities[0].score)+'\n')
        self.win.refresh()

    def end(self):
        curses.endwin()

    def getEvent(self):
        return self.win.getch()

class MapViewer:
    def __init__(self, m, w):
        self.map = m
        self.world = w
        self.cellViews = [CellViewer(c) for c in self.map.cells]
        
    def display(self, win):
        win.clear()
        # image de fond ici
        for x in range(self.map.width+3):
            win.addch(0, x, 35)
            win.addch(self.map.height+2, x, 35)
        for y in range(1, self.map.height+2):
            win.addch(y, 0, 35)
            win.addch(y, self.map.width+2, 35)
        #for c in self.cellViews: c.display(win)
        for ent in self.world.entities:
            win.addch(ent.y+1, ent.x+1, ent.picture)
        for ent in self.world.objects:
            win.addch(ent.y+1, ent.x+1, ent.picture)
        # TODO gérer plusieurs cartes

class CellViewer:
    def __init__(self, cell):
        self.cell = cell
    
    def display(self, win):
        win.addch(self.cell.y+1, self.cell.x+1, self.cell.picture)
