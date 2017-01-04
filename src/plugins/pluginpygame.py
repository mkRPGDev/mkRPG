"""
    This module only define the class PygamePlugin,
    which is a class to write a pygame plugin
"""
import pygame

class PygamePlugin(pygame.sprite.Sprite):
    """ Subclass this to write a pygame plugin, which is a layer that should
    be drawn on screen over the map"""

    def __init__(self, netPlugin):
        self.netPlugin = netPlugin
        self.image = None # the sprite image
        self.rect = None # the sprite size and position

    def handleEvent(self, event):
        """ return True if the key has been used """
        return False

    def draw(self):
        """ creates/updates the Surface to blit """
        pass

    def update(self):
        """ do updates """
        pass
