from interface.interface import Interface, skeys

from itertools import chain
import curses

class Curses(Interface):
    """ ncurses-based UI """
    def __init__(self, w, p):
        super().__init__(w, p)
        self.win = curses.initscr()
        curses.cbreak()
        curses.noecho()
        curses.curs_set(0)
        self.win.keypad(True)
        self.win.nodelay(True)
        xmin, xmax, ymin, ymax = 0, curses.COLS, 0, curses.LINES
#        xmax = xmax - MenuView.MINW
#        self.menuView = MenuView(curses.newwin(curses.LINES, MenuView.MINW,
#                                               0, curses.COLS-MenuView.MINW), w, self.plugins)
        for p in self.plugins:
            # les premiers plugins sont prioritaires en taille
            p.win = curses.newwin(min(p.MINH, ymax), min(p.MINW, xmax), 
                                  p.Y if p.Y>=0 else curses.LINES+p.Y,
                                  p.X if p.X>=0 else curses.COLS+p.X)
            xmin = max(xmin, p.X if p.X>=0 else 0)
            xmax = min(xmax, xmax if p.X>=0 else curses.COLS+p.X)
            ymin = max(ymin, p.Y if p.Y>=0 else 0)
            ymax = min(ymax, ymax if p.Y>=0 else curses.LINES+p.Y)
            p.interface = self
        self.mapView = MapView(curses.newwin(ymax-ymin, xmax-xmin, ymin, xmin), w)
        
        
    def init(self): # eurk !
        self.getEvent()
        self.repaint()
    
    def setPerso(self, perso):
        self.mapView.perso = perso
    
    def repaint(self):
#        self.menuView.draw()
        self.mapView.draw()
        for p in self.plugins:
            p.draw()
        curses.doupdate()
        # TODO insérer ici le xml d'interface
        #self.win.addstr(26,0,"Score "+str(self.world.entities[0].score)+'\n')

    def end(self):
        curses.endwin()

    def getEvent(self):
        key=self.win.getch()
        if key==-1: return []
        for p in self.plugins:
            if p.handleKey(key):
                self.repaint()
                return []
        if key==ord('w'): return [skeys.QUIT]
        if key==ord('p'): return [skeys.PAUSE]
        if key==ord('r'): return [skeys.RESUME]
        if self.mapView.handleKey(key):
            self.repaint()
            return []
        return [key]


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
        self.perso = None#self.world.entities[0]
        self.follow = False
 
    def draw(self):
#        win = self.mapWin
        if self.map != self.world.currentMap:
            self.map = self.world.currentMap
            print("map is %s " % self.map)
            self.height = min(self.map.height, self.maxHeight-2)
            self.width = min(self.map.width, self.maxWidth-2)
#            win.resize(self.height+2, self.width+2)
        win = self.win.derwin(self.height+2, self.width+2, 
                              (self.maxHeight-self.height-2)//2,
                              (self.maxWidth-self.width-2)//2)
        win.erase()
        if self.follow:
            self.offX = self.perso.x - self.width//2 
            self.offY = self.perso.y - self.height//2
            self.clipOffset() 
        # Borders
        chh = 0 if not self.offY else 94
        chb = 0 if self.offY==self.map.height-self.height else 118
        chg = 0 if not self.offX else 60
        chd = 0 if self.offX==self.map.width-self.width else 62
        win.border(chg, chd, chh, chb)
        # Cells
        if self.showLov: self.fastLov()
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
        if self.showLov and cell.picture==32 and self.lovs[cell.x-self.offX][cell.y-self.offY]:
            return '.'
        else:
            return chr(cell.picture)

    def clipOffset(self):
        self.offX = max(0, min(self.offX,self.map.width-self.width))
        self.offY = max(0, min(self.offY,self.map.height-self.height))

    def handleKey(self, key):
        if key==curses.KEY_LEFT: self.offX -= 1
        elif key==curses.KEY_RIGHT: self.offX += 1
        elif key==curses.KEY_UP: self.offY -= 1
        elif key==curses.KEY_DOWN: self.offY += 1
        elif key==ord('l'): self.showLov ^= 1
        elif key==ord('f'): self.follow ^= 1
        else: return False
        self.clipOffset()
        return True
    
    def fastLov(self):
        """ as Map.lov but optimised for a zone ~O(cellNumber)
        rather fluid up to 20.000 cells """
        def cadran(dx, dy, cx, cy):
            """ compute the shadow of (dx,dy) up to (cx,cy) """
            rx,ry = px,py # local copy
            if dx>cx: rx,cx,dx = -rx,-cx,-dx
            if dy>cy: ry,cy,dy = -ry,-cy,-dy
            start = dy
            for x in range(dx,cx+1):
                seen = False
                for y in range(start,cy+1):
                    if (2*dx-1-2*rx)*(y-ry) < (2*dy+1-2*ry)*(x-rx) and \
                       (2*dx+1-2*rx)*(y-ry) > (2*dy-1-2*ry)*(x-rx):
                        lovs[abs(x)][abs(y)] = False
                        seen = True
                    elif seen: break
                    else: start = y

        px, py = self.perso.x-self.offX, self.perso.y-self.offY
        lovs = [[True]*self.height for _ in range(self.width)]
        for cell in self.map.fromPos(self.perso, self.height+self.width):
            dx,dy = cell.x-self.offX, cell.y-self.offY
            if dx not in range(self.width) or dy not in range(self.height):
                continue
            if not lovs[dx][dy] or not cell.visible: continue
            # p -> perso, d -> obstacle, c -> coin de la vue
            if (dx,dy) == (px,py): continue
            cy = 0 if dy<py else self.height-1
            if dx<=px: cadran(dx, dy, 0, cy)
            if dx>=px: cadran(dx, dy, self.width-1, cy)
            if dy==py: cadran(dx, dy, 0 if dx<px else self.width-1, 0)
        self.lovs = lovs
    
