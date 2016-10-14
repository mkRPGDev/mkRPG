# -*- coding: utf-8 -*-

import configparser

import pygame
from pygame.locals import *

from map import Map
import const

class Client():
    
    def __init__(self):
        self.get_conf_file("client_conf.ini")
        
        pygame.display.init()
        self.screen_size = (self.get_conf("width", int), self.get_conf("height", int))
        screen = pygame.display.set_mode(self.screen_size)

        self.current_map = Map(self.screen_size, const.IMG_PATH+"map0.png")
        
        self.background = pygame.Surface(self.screen_size)
        self.background = self.background.convert()
        self.background.fill((0, 0, 0))
        
        pygame.font.init()
        font = pygame.font.Font(None, 18)
        
        clock = pygame.time.Clock()

        while(1):
            clock.tick(60)
            
            for event in pygame.event.get():
                if event.type == QUIT:
                    exit(0)
                elif event.type == MOUSEBUTTONDOWN and event.button == 4:
                    self.current_map.zoom(1+self.get_conf("zoom_step",float))
                elif event.type == MOUSEBUTTONDOWN and event.button == 5:
                    self.current_map.zoom(1-self.get_conf("zoom_step",float))
                elif event.type == KEYDOWN:
                    if event.key == K_LEFT:
                        self.current_map.move(-self.get_conf("mov_step", int), 0)
                    elif event.key == K_RIGHT:
                        self.current_map.move(self.get_conf("mov_step", int), 0)
                    elif event.key == K_UP:
                        self.current_map.move(0, self.get_conf("mov_step", int))
                    elif event.key == K_DOWN:
                        self.current_map.move(0, -self.get_conf("mov_step", int))
            
            text = font.render("FPS : %d" % clock.get_fps(), 1, (255,0,0))
            self.current_map.update()
            
            screen.blit(self.background, (0,0))
            screen.blit(self.current_map.render(), self.current_map.pos_offset)
            screen.blit(text, (10,10))
            
            pygame.display.update(Rect((0,0), self.screen_size))
        
        pygame.display.quit()
    
    def get_conf_file(self, path):
        self.conf = configparser.ConfigParser()
        self.conf.read(path)
    
    def get_conf(self, conf, type=str):
        return type(self.conf.get("CURRENT", conf))

client = Client()