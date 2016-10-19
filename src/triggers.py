# -*- coding: utf-8 -*-

class MoveTrigger(Exception):

    def __init__(self, target):
        self.target = target