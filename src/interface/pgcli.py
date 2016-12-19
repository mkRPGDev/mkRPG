from math import sin,cos,pi
from itertools import chain
from pygame.locals import QUIT, KEYDOWN, VIDEORESIZE, RESIZABLE, \
                          K_ESCAPE, K_UP, K_DOWN, K_RIGHT, K_LEFT, K_PAGEUP, K_PAGEDOWN
import pygame

from interface.const import SCREEN_WIDTH, SCREEN_HEIGHT
from interface.interface import Interface, skeys
from interface.utils import load_png
from interface.trans import applyMatrix

from time import time

IMGS = {32:"herbe.png",79:"head.png",88:"head.png",120:"apple.png",111:"body.png"}

class Pygame(Interface):
    """ pygame-based UI """
    def __init__(self, *args):
        super().__init__(*args)
        self.mapView = MapView(self.world)

        pygame.display.init()
        pygame.key.set_repeat(50,20)
        
        self.resize(SCREEN_WIDTH, SCREEN_HEIGHT)
        self.clock = pygame.time.Clock()
        pygame.font.init()
        self.font = pygame.font.Font(None, 18)

    def resize(self, w, h):
        # FIXME lors d'un agrandissement brutal, l'event ne suit pas toujours
        self.screen = pygame.display.set_mode((w,h), RESIZABLE)
        # TODO laisser de la place aux plugins
        self.mapView.setSurf(self.screen.subsurface((0, 0, w-1, h-1)))
        
    def init(self): # eurk !
        self.getEvent()
        self.repaint()
    
    def setPerso(self, perso):
        self.mapView.perso = perso
    
    def repaint(self):
        self.clock.tick()
        self.mapView.draw()
#       for p in self.plugins:
#           p.draw()
        text = self.font.render("FPS : %d" % self.clock.get_fps(), 1, (255,0,0))
        self.screen.blit(text, (10,10))
        pygame.display.flip()

    def end(self):
        pygame.display.quit()

    def getEvent(self):
        evs = pygame.event.get()
        for i, ev in enumerate(evs):
            if ev.type==QUIT: evs[i]=skeys.QUIT
            elif ev.type==KEYDOWN:
                key=ev.key
#                for p in self.plugins:
#                    if p.handleKey(key):
#                        self.repaint()
#                        evs[i]=None
#                        continue
                if key==K_ESCAPE: evs[i]=skeys.QUIT
                elif key==ord('p'): evs[i]=skeys.PAUSE
                elif key==ord('r'): evs[i]=skeys.RESUME
                elif self.mapView.handleKey(key):
                    self.repaint()
                    evs[i]=None
                else:
                    evs[i]=key
            elif ev.type==VIDEORESIZE:
                print(ev.w, ev.h)
                self.resize(ev.w, ev.h)
                self.repaint()
                evs[i]=None
            else: evs[i]=None
        return list(filter(None, evs))


class MapView:
    """ Manages the map display """
    def __init__(self, world):
        self.world = world
        self.offX = 0
        self.offY = 0
        self.angleX = pi/8#ANGLE_X_R
        self.angleY = -pi/8#ANGLE_Y_R
        self.cellWidth = 32
        self.map = None
        self.perso = None
        self.showLov = False
        self.follow = False
        self.pics={}
        
    def setSurf(self, surf):
        """ Called when scene is initialised or reshaped """
        self.surf = surf
        self.maxWidth, self.maxHeight = self.surf.get_size()
        self.updateMap()
        self.updatePics()
        self.clipOffset()
    
    def updatePics(self):
        """ Fill the picture dictionnary using current angles and zoom """
        self.cellWidth = max(3, min(self.cellWidth, 90))
        for i in IMGS:
            p = load_png("../content/imgs/"+IMGS[i])
            p = pygame.transform.scale(p, (int(self.cellWidth+3), int(self.cellWidth+3)))
            p = applyMatrix(p, [-cos(self.angleX),sin(self.angleX),-cos(self.angleY),sin(self.angleY)])
            self.pics[i] = p
    
    def updateMap(self):
        self.map = self.world.currentMap
        self.nbCellsX = self.map.width
        self.nbCellsY = self.map.height
            
    def clipOffset(self):
        """ Ensure the offX, offY lead to a valid display """
        x1,y1 = self.cellToPoint(-.5, -.5) # position of transformed angles
        x2,y2 = self.cellToPoint(self.map.width+.5, -.5)
        x3,y3 = self.cellToPoint(-.5, self.map.height+.5)
        x4,y4 = self.cellToPoint(self.map.width+.5, self.map.height+.5)
        xm = min(x1, x2, x3, x4) # computation of the required size
        ym = min(y1, y2, y3, y4)
        xn = max(x1, x2, x3, x4)
        yn = max(y1, y2, y3, y4)
        self.offX = max(xm, min(self.offX, xn-self.maxWidth))
        self.offY = max(ym, min(self.offY, yn-self.maxHeight))
        self.updateVisibleScene()

    def updateVisibleScene(self):
        """ Updates self.shown, a list of visible cells """
        sceneRect = self.surf.get_rect()
        sceneRect.inflate_ip(self.cellWidth*3, self.cellWidth*3) #3>2*sqrt(2)
        start=0
        t=time()
        # position of transformed angles
        x1,y1 = self.pointToCell(self.offX,               self.offY) 
        x2,y2 = self.pointToCell(self.maxWidth+self.offX, self.offY)
        x3,y3 = self.pointToCell(self.offX,               self.maxHeight+self.offY)
        x4,y4 = self.pointToCell(self.maxWidth+self.offX, self.maxHeight+self.offY)
        # computation of the needed size
        xm = min(x1, x2, x3, x4)
        ym = min(y1, y2, y3, y4)
        xn = max(x1, x2, x3, x4)
        yn = max(y1, y2, y3, y4)
        xm = max(xm-1, 0)
        ym = max(ym-1, 0)
        xn = min(xn+1, self.nbCellsX)
        yn = min(yn+1, self.nbCellsY)
        
        i,j=0,0
        self.shown = []
        for x in range(xm,xn):
            seen=False
            for y in range(ym, yn):
                i+=1
                u,v = self.cellToPoint(x,y)
                u -= self.offX
                v -= self.offY
                if sceneRect.collidepoint(u,v):
                    self.shown.append((u,v))
                    j+=1
                    seen = True
                elif seen: break
        print(i,j,time()-t)
        
    def draw(self):
        """ Blit visible items of the map on Surface self.surf """
        if self.map != self.world.currentMap: self.updateMap()
        self.surf.fill((0,0,0))
        if self.follow:
            x,y = self.cellToPoint(self.perso.x, self.perso.y)
            self.offX = x - self.maxWidth//2 
            self.offY = y - self.maxHeight//2
            self.clipOffset() 
        t=time()
        # Cells
        for u,v in self.shown:
            self.surf.blit(self.pics[32], (u,v))
        print(time()-t)
        # Entities and objects
        sceneRect = self.surf.get_rect()
        for ent in chain(self.world.entities, self.world.objects):
            u,v = self.cellToPoint(ent.x, ent.y)
            u -= self.offX
            v -= self.offY
            if sceneRect.collidepoint(u,v):
                self.surf.blit(self.pics[ent.picture], (u,v))

    def handleKey(self, key):
        if key==K_LEFT: self.offX -= 100
        elif key==K_RIGHT: self.offX += 100
        elif key==K_UP: self.offY -= 100
        elif key==K_DOWN: self.offY += 100
        elif key==K_PAGEUP: self.cellWidth /= 1.5
        elif key==K_PAGEDOWN: self.cellWidth *= 1.5
        elif key==ord('u'): self.angleX -= pi/16
        elif key==ord('i'): self.angleX += pi/16
        elif key==ord('j'): self.angleY -= pi/16
        elif key==ord('k'): self.angleY += pi/16
#        elif key==ord('l'): self.showLov ^= 1
        elif key==ord('f'): self.follow ^= 1
        else: return False
        self.updatePics()
        self.clipOffset()
        return True
    
    def cellToPoint(self, x, y):
        """ Convert cell related coordinates to map coordinates """
        # XXX precompute sin and cos ?
        return (( x               *cos(self.angleX) - (self.nbCellsY-y)*cos(self.angleY))*self.cellWidth,
                ((self.nbCellsX-x)*sin(self.angleX) + (self.nbCellsY-y)*sin(self.angleY))*self.cellWidth)
    
    def pointToCell(self, x, y):
        """ Convert map coordinates to cell related coordinates """
        d = -cos(self.angleX)*sin(self.angleY)+sin(self.angleX)*cos(self.angleY)
        x += self.nbCellsY*cos(self.angleY)*self.cellWidth
        y += -(self.nbCellsX*sin(self.angleX)+self.nbCellsY*sin(self.angleY))*self.cellWidth
        return (round((-sin(self.angleY)*x -cos(self.angleY)*y)/d/self.cellWidth),
                round((+sin(self.angleX)*x +cos(self.angleX)*y)/d/self.cellWidth))
