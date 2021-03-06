"""
    This module defines the UI base-class
"""
from time import time
from enum import IntEnum

from shared.const import UPDTIME


skeys = IntEnum("keys", "QUIT PAUSE RESUME")

class Interface:
    """ UI base-class, can be used as a dummy interface """
    def __init__(self, w, i, p):
        self.world = w
        self.lastUpdate = 0
        self.plugins = p
        self.images = i

    def update(self):
        """ Update the UI """
        if time() - self.lastUpdate > UPDTIME:
            self.repaint()
            self.lastUpdate = time()

    def repaint(self):
        pass
    def setPerso(self, p):
        pass
    def init(self):
        pass

    def end(self):
        pass

    def getEvent(self):
        return []#-1 #while True: await sleep(1)
