"""
    Pygame based interface
"""
from math import sin, cos, pi
from itertools import chain
from pygame.locals import FULLSCREEN, RESIZABLE, ACTIVEEVENT,\
                          QUIT, KEYDOWN, VIDEORESIZE, MOUSEBUTTONDOWN,\
                          K_ESCAPE, K_UP, K_DOWN, K_RIGHT, K_LEFT, K_PAGEUP,\
                          K_PAGEDOWN, K_F11
import pygame

from interface.const import *
from interface.interface import Interface, skeys
from interface.utils import loadPng
from interface.applymatrix import applyMatrix

from time import time

class Pygame(Interface):
    """ pygame-based UI """
    def __init__(self, *args):
        super().__init__(*args)
        self.mapView = MapView(self.world, {i:v.image for i, v in self.images.items()})

        pygame.display.init()
        pygame.key.set_repeat(50, 20)
        self.fullscreen = None # ancienne dim si fullscreen

        self.resize(SCREEN_WIDTH, SCREEN_HEIGHT)
        self.clock = pygame.time.Clock()
        pygame.font.init()
        self.font = pygame.font.Font(None, 18)

    def resize(self, w, h):
        """ change screen size """
        # FIXME lors d'un agrandissement brutal, l'event ne suit pas toujours
        # Fix possible : interdire le changement de résolution en cours de jeu
        self.screen = pygame.display.set_mode((w, h),
                                              (FULLSCREEN if self.fullscreen else RESIZABLE))
        # TODO laisser de la place aux plugins
        if (w, h) == (0, 0): #plein écran
            w, h = self.screen.get_size()
        self.mapView.setSurf(self.screen.subsurface((0, 0, w-1, h-1)))

    def init(self): # eurk !
        """ empty events queue and draw screen """
        self.getEvent()
        self.repaint()

    def setPerso(self, perso):
        """ sets current character """
        self.mapView.perso = perso

    def repaint(self):
        """ draw everything on screen """
        deltat = self.clock.tick()
        self.mapView.draw(deltat)
        for p in self.plugins:
            p.draw()
            self.screen.blit(p.image, (p.rect.x, p.rect.y))
        text = self.font.render("FPS : %d" % self.clock.get_fps(), 1, (255, 0, 0))
        self.screen.blit(text, (10, 10))
        pygame.display.flip()

    def end(self):
        """ quit the game """
        pygame.display.quit()

    def getEvent(self):
        """ get all the events in pygame events queue """
        evs = pygame.event.get()
        for i, ev in enumerate(evs):
            if ev.type == QUIT:
                evs[i] = skeys.QUIT
            elif ev.type in (KEYDOWN, MOUSEBUTTONDOWN):
                # les numéros de touche/boutton n'ont pas de collisions
                key = ev.key if ev.type == KEYDOWN else ev.button
#                for p in self.plugins:
#                    if p.handleKey(key):
#                        self.repaint()
#                        evs[i]=None
#                        break
                if key == K_ESCAPE:
                    evs[i] = skeys.QUIT
                elif key == ord('p'):
                    evs[i] = skeys.PAUSE
                elif key == ord('r'):
                    evs[i] = skeys.RESUME
                elif key == K_F11:
                    if self.fullscreen:
                        dim = self.fullscreen
                        self.fullscreen = None
                        self.resize(*dim)
                    else:
                        self.fullscreen = self.screen.get_size()
                        self.resize(1920, 1080)
                        # FIXME mais utiliser pygame (0,0) ne marche pas
                elif self.mapView.handleKey(key):
                    self.repaint()
                    evs[i] = None
                elif ev.type == KEYDOWN:
                    evs[i] = key
                else:
                    evs[i] = None
                    for p in self.plugins:
                        p.handleEvent(ev)
            elif ev.type == VIDEORESIZE:
                self.resize(ev.w, ev.h)
                self.repaint()
                evs[i] = None
            elif ev.type == ACTIVEEVENT:
                self.mapView.active = ev.gain
                evs[i] = None
            else: evs[i] = None
        return list(filter(None, evs))


class MapView:
    """ Manages the map display """
    def __init__(self, world, imgs):
        self.world = world
        self.offX, self.offY = 0, 0
        self.movSpeedX, self.movSpeedY = 0, 0
        self.angle = ANGLE
        self.flattening = FLATTENING
        self.angleX = None
        self.angleY = None
        self.update_angles()
        self.cellWidth = CELL_WIDTH
        self.map = None
        self.perso = None
        self.showLov = False
        self.follow = False
        self.active = False
        self.pics = {}
        self.imgs = imgs


    def setSurf(self, surf):
        """ Called when scene is initialised or reshaped """
        self.surf = surf
        self.maxWidth, self.maxHeight = self.surf.get_size()
        self.updateMap()
        self.updatePics()
        self.clipOffset()

    def updatePics(self):
        """ Fill the picture dictionnary using current angles and zoom """
        lar = cos(self.angleY)*(self.nbCellsY+1)+cos(self.angleX)*(self.nbCellsX+1)
        self.cellWidth = max(self.maxWidth/lar, min(self.cellWidth, 90))
        for i in self.imgs:
            p = loadPng(self.imgs[i])
            p = pygame.transform.scale(p, (int(self.cellWidth+3), int(self.cellWidth+3)))
            p2 = applyMatrix(p, [-cos(self.angleX), sin(self.angleX),
                                -cos(self.angleY), sin(self.angleY)])
            self.pics[i] = (p, p2)

    def updateMap(self):
        """ sets current map to what it should be """
        self.map = self.world.currentMap
        self.nbCellsX = self.map.width
        self.nbCellsY = self.map.height

    def clipOffset(self):
        """ Ensure the offX, offY lead to a valid display """
        x1, y1 = self.cellToPoint(-.5, -.5) # position of transformed angles
        x2, y2 = self.cellToPoint(self.map.width+.5, -.5)
        x3, y3 = self.cellToPoint(-.5, self.map.height+.5)
        x4, y4 = self.cellToPoint(self.map.width+.5, self.map.height+.5)
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
        sceneRect.inflate_ip(self.cellWidth*4, self.cellWidth*4)
        start = 0
        t = time()
        # position of transformed angles
        x1, y1 = self.pointToCell(self.offX, self.offY)
        x2, y2 = self.pointToCell(self.maxWidth+self.offX, self.offY)
        x3, y3 = self.pointToCell(self.offX, self.maxHeight+self.offY)
        x4, y4 = self.pointToCell(self.maxWidth+self.offX, self.maxHeight+self.offY)
        # computation of the needed size
        xm = min(x1, x2, x3, x4)
        ym = min(y1, y2, y3, y4)
        xn = max(x1, x2, x3, x4)
        yn = max(y1, y2, y3, y4)
        xm = max(xm-1, 0)
        ym = max(ym-1, 0)
        xn = min(xn+1, self.nbCellsX)
        yn = min(yn+1, self.nbCellsY)

        i, j = 0, 0
        self.shown = []
        for x in range(xm, xn):
            seen = False
            for y in range(ym, yn):
                i += 1
                u, v = self.cellToPoint(x, y)
                u -= self.offX
                v -= self.offY
                if sceneRect.collidepoint(u, v):
                    self.shown.append((u, v, self.map.cellsGrid[x][y]))
                    j += 1
                    seen = True
                elif seen: break
        #print(i,j,time()-t)

    def draw(self, deltat=0):
        """ Blit visible items of the map on Surface self.surf """
        if self.map != self.world.currentMap: self.updateMap()
        self.moveView(deltat)
        self.surf.fill((0, 0, 0))
        if self.follow:
            x, y = self.cellToPoint(self.perso.x, self.perso.y)
            self.offX = x - self.maxWidth//2
            self.offY = y - self.maxHeight//2
            self.clipOffset()
        t = time()
        # Cells
        for u, v, cell in self.shown:
            self.surf.blit(self.pics[cell.picture][1], (u, v))
        # Entities and objects
        sceneRect = self.surf.get_rect()
        for ent in chain(self.world.entities, self.world.objects):
            u, v = self.cellToPoint(ent.x, ent.y)
            u -= self.offX
            v -= self.offY
            if sceneRect.collidepoint(u, v):
                if "transform" in ent.params:
                    self.surf.blit(self.pics[ent.picture][ent.transform], (u, v))
                else:
                    self.surf.blit(self.pics[ent.picture][0], (u, v))
        if self.movSpeedX != 0:
            mask = pygame.Surface((MOV_OFFSET, self.maxHeight))
            mask.fill((255, 255, 255))
            mask.set_alpha(50)
            self.surf.blit(mask, (0 if self.movSpeedX < 0 else self.maxWidth-MOV_OFFSET, 0))
        if self.movSpeedY != 0:
            mask = pygame.Surface((self.maxWidth, MOV_OFFSET))
            mask.fill((255, 255, 255))
            mask.set_alpha(50)
            self.surf.blit(mask, (0, 0 if self.movSpeedY < 0 else self.maxHeight-MOV_OFFSET))

    def handleKey(self, key):
        """ return true if key has been handled else false """
        if key == K_LEFT:
            self.offX -= 100
        elif key == K_RIGHT:
            self.offX += 100
        elif key == K_UP:
            self.offY -= 100
        elif key == K_DOWN:
            self.offY += 100
        elif key == 4:
            self.cellWidth /= 1.5
        elif key == 5:
            self.cellWidth *= 1.5
#        elif key==ord('l'): self.showLov ^= 1
        elif key == ord('f'):
            self.follow ^= 1
        else:
            return False
        self.updatePics()
        self.clipOffset()
        return True

    def moveView(self, deltat):
        """ Move view with accelerations if the mouse is on an edge """
        if not self.active or not pygame.key.get_focused():
            self.movSpeedX, self.movSpeedY = 0, 0
            return
        posX, posY = pygame.mouse.get_pos()

        maxSpeed = 600 # XXX fonction du zoom ?
        movAccel = int(0.1*deltat)

        if posX in range(MOV_OFFSET) and -self.movSpeedX in range(maxSpeed):
            self.movSpeedX -= movAccel
        elif self.maxWidth-posX in range(MOV_OFFSET) and self.movSpeedX in range(maxSpeed):
            self.movSpeedX += movAccel
        else:
            self.movSpeedX = 0

        if posY in range(MOV_OFFSET) and -self.movSpeedY in range(maxSpeed):
            self.movSpeedY -= movAccel
        elif self.maxHeight-posY in range(MOV_OFFSET) and self.movSpeedY in range(maxSpeed):
            self.movSpeedY += movAccel
        else:
            self.movSpeedY = 0

        if self.movSpeedX or self.movSpeedY:
            self.follow = False
            self.offX += self.movSpeedX
            self.offY += self.movSpeedY
            self.clipOffset()

    def cellToPoint(self, x, y):
        """ Convert cell related coordinates to map coordinates """
        # XXX precompute sin and cos ?
        return ((x*cos(self.angleX) - (self.nbCellsY-y)*cos(self.angleY))*self.cellWidth,
                ((self.nbCellsX-x)*sin(self.angleX) +
                 (self.nbCellsY-y)*sin(self.angleY))*self.cellWidth)

    def pointToCell(self, x, y):
        """ Convert map coordinates to cell related coordinates """
        d = -cos(self.angleX)*sin(self.angleY)+sin(self.angleX)*cos(self.angleY)
        if d == 0 : d = 0.1
        x += self.nbCellsY*cos(self.angleY)*self.cellWidth
        y += -(self.nbCellsX*sin(self.angleX)+self.nbCellsY*sin(self.angleY))*self.cellWidth
        return (round((-sin(self.angleY)*x -cos(self.angleY)*y)/d/self.cellWidth),
                round((+sin(self.angleX)*x +cos(self.angleX)*y)/d/self.cellWidth))

    def rotate(self, step):
        """
            Rotates the map

            @param step the angle step to be applied, may be negative
        """
        if self.angle + step > 15 and self.angle + step < 75:
            self.angle += step
            self.update_angles()
            self.updateVisibleScene()

    def flatten(self, step):
        """
            Flattens the map

            @param step the flattening step to be applied, may be negative
        """
        if self.flattening + step > -.8 and self.flattening + step < .8:
            self.flattening += step
            self.update_angles()
            self.updateVisibleScene()

    def update_angles(self):
        """ Updates the anglesX/Y according to angle and flattening parameters """
        self.angleX = self.angle * (1 - abs(self.flattening)) + \
            90 * max(0, self.flattening)
        self.angleY = self.angle * (1 - abs(self.flattening)) + \
            90 * max(0, 1 - self.flattening)
