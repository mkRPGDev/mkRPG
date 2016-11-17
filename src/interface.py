from time import time

from const import *

class Interface:
    """ UI base-class, can be used as a dummy interface """
    def __init__(self, w):
        self.world = w
        self.lastUpdate = 0

    def update(self):
        if time() - self.lastUpdate > UPDTIME:
            self.repaint()
            self.lastUpdate = time()
            
    def repaint(self): pass

    def end(self): pass

    def getEvent(self):
        return -1 #while True: await sleep(1)
