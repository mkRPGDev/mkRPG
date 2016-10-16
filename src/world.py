# -*- coding: utf-8 -*-

import pygame
from pygame.locals import *

from map import Map
import const

class World():

    def __init__(self, screen_size):
        self.screen_size = screen_size
        self.current_map = Map(self.screen_size, "bg0.bg")