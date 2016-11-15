from time import time, sleep
from const import *

class Interface:
    """ UI base-class, can be used as a dummy interface """
    def __init__(self, w):
        self.world = w
        self.lastUpdate = 0

    def update(self):
        if time() - self.lastUpdate > 1/MAXFPS:
            self.repaint()
            self.lastUpdate = time()
            
    def repaint(self): pass

    def end(self): pass

    def getEvent(self):
        while True: sleep(1)

