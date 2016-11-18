from interface import Interface, skeys

from itertools import chain
import curses

class Curses(Interface):
    """ ncurses-based UI """
    def __init__(self, w):
        super().__init__(w)
        self.win = curses.initscr()
        curses.cbreak()
        curses.noecho()
        curses.curs_set(0)
        self.win.keypad(True)
        self.win.nodelay(True)
        self.mapView = MapView(curses.newwin(curses.LINES-4, curses.COLS-MenuView.MINW, 2, 0), w)
        self.menuView = MenuView(curses.newwin(curses.LINES, MenuView.MINW, 0, curses.COLS-MenuView.MINW), w)

    def init(self): # eurk !
        self.getEvent()
        self.repaint()
        
    def repaint(self):
        self.menuView.draw()
        self.mapView.draw()
        curses.doupdate()
        # TODO insérer ici le xml d'interface
        #self.win.addstr(26,0,"Score "+str(self.world.entities[0].score)+'\n')

    def end(self):
        curses.endwin()

    def getEvent(self):
        key=self.win.getch()
        if key==-1: return []
        if key==ord('w'): return [skeys.QUIT]
        if key==ord('p'): return [skeys.PAUSE]
        if key==ord('r'): return [skeys.RESUME]
        if self.mapView.handleKey(key):
            self.repaint()
            return []
        return [key]

class MenuView:
    """ manages the curses menu windows """
    MINW = 26
    def __init__(self, win, world):
        self.win = win
        self.world = world
        self.height, self.width = self.win.getmaxyx()

    def draw(self):
        self.win.border()
        self.centered(3, "MKRPG", curses.A_BOLD)    
        self.centered(5, "The curses client", curses.A_BOLD)
        self.win.addstr(9,  2, b"[r] : start/resume")
        self.win.addstr(10, 2, b"[p] : pause")
        self.win.addstr(12, 2, b"[w] : quit")
        self.win.addstr(14,  2, b"arrows : move map view")
        self.win.addstr(16,  2, b"[l] : show/hide")
        self.win.addstr(17,  2, b"      lignes of view")
#        for i,c in enumerate((curses.ACS_LARROW, curses.ACS_DARROW,curses.ACS_UARROW, curses.ACS_RARROW)):
#            self.win.addch(14, 3+i, c)
        self.centered(self.height-2, "the mkrpg team")
        self.win.noutrefresh()
    
    def centered(self, y, st, attr = None):
        """ same as addstr but centered """
        if len(st)>self.width: return
        if attr:
            self.win.addstr(y, (self.width-len(st))//2, st, attr)
        else: 
            self.win.addstr(y, (self.width-len(st))//2, st)

class MapView:
    """ manages the map display """
    def __init__(self, win, world):
        self.win = win
        self.world = world
        self.offX = 0
        self.offY = 0
        #self.mapWin = self.win.derwin(0,0)
        self.map = None
        self.maxHeight, self.maxWidth = self.win.getmaxyx()
        self.showLov = False
        
    def draw(self):
#        win = self.mapWin
        if self.map != self.world.currentMap:
            self.map = self.world.currentMap
            self.height = min(self.map.height, self.maxHeight-2)
            self.width = min(self.map.width, self.maxWidth-2)
#            win.resize(self.height+2, self.width+2)
        win = self.win.derwin(self.height+2, self.width+2, (self.maxHeight-self.height-2)//2, (self.maxWidth-self.width-2)//2)
        win.erase()
        # Borders
        chh = 0 if not self.offY else 94
        chb = 0 if self.offY==self.map.height-self.height else 118
        chg = 0 if not self.offX else 60
        chd = 0 if self.offX==self.map.width-self.width else 62
        win.border(chg, chd, chh, chb)
        # Cells
        for y in range(self.offY, self.offY+self.height):
            win.addstr(y-self.offY+1, 1,
              "".join(self.cellChar(self.map.cellsGrid[x][y]) for x in range(self.offX, self.offX+self.width)))
        # Entities and objects
        for ent in chain(self.world.entities, self.world.objects):
            x, y = ent.x-self.offX, ent.y-self.offY
            if x in range(self.width) and y in range(self.height):
                win.addch(y+1, x+1, ent.picture)
        self.win.noutrefresh()

    def cellChar(self, cell):
        if self.showLov and cell.picture==32 and self.map.lov(cell, self.world.entities[0]):
            return '.'
        else:
            return chr(cell.picture)

    def handleKey(self, key):
        if key==curses.KEY_LEFT:
            self.offX = max(self.offX-1,0)
        elif key==curses.KEY_RIGHT:
            self.offX = min(self.offX+1,self.map.width-self.width)
        elif key==curses.KEY_UP:
            self.offY = max(self.offY-1,0)
        elif key==curses.KEY_DOWN:
            self.offY = min(self.offY+1,self.map.height-self.height)
        elif key==ord('l'):
            self.showLov = not self.showLov
        else:
            return False
        return True
