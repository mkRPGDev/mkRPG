# -*- coding: utf-8 -*-

import configparser

import pygame
from pygame.locals import *

from world import World
import const

class Client():
    
    def __init__(self):
        self.get_conf_file("client_conf.ini")
        
        pygame.display.init()
        self.screen_size = (self.get_conf("width", int), self.get_conf("height", int))
        screen = pygame.display.set_mode(self.screen_size)
        
        self.world = World(self.screen_size)
        
        self.background = pygame.Surface(self.screen_size)
        self.background = self.background.convert()
        self.background.fill((0, 0, 0))
        
        pygame.font.init()
        font = pygame.font.Font(None, 18)
        
        clock = pygame.time.Clock()
        
        mov_speed_x = 0
        mov_speed_y = 0
        zoom_speed = 0

        while(1):
            deltat = clock.tick(60)/1000
            
            for event in pygame.event.get():
                if event.type == QUIT:
                    exit(0)
                elif event.type == MOUSEBUTTONDOWN and event.button == 4:
                    if (zoom_speed == 0 or zoom_speed > 0) and zoom_speed < self.get_conf("max_zoom_speed", float):
                        zoom_speed += self.get_conf("zoom_speed",float)*deltat
                    else:
                        zoom_speed = 0
                elif event.type == MOUSEBUTTONDOWN and event.button == 5:
                    if (zoom_speed == 0 or zoom_speed < 0) and zoom_speed > -self.get_conf("max_zoom_speed", float):
                        zoom_speed -= self.get_conf("zoom_speed",float)*deltat
                    else:
                        zoom_speed = 0
                elif event.type == MOUSEBUTTONUP and event.button == 1:
                    self.propagate_trigger(event)
            (posx,posy) = pygame.mouse.get_pos()
            if posx < const.MOV_OFFSET:
                if (mov_speed_x == 0 or mov_speed_x > 0) and mov_speed_x < self.get_conf("max_mov_speed", int):
                    mov_speed_x += int(self.get_conf("mov_speed", int)*deltat)
                else:
                    mov_speed_x = 0
            elif posx > self.screen_size[0]-const.MOV_OFFSET:
                if (mov_speed_x == 0 or mov_speed_x < 0) and mov_speed_x > -self.get_conf("max_mov_speed", int):
                    mov_speed_x -= int(self.get_conf("mov_speed", int)*deltat)
                else:
                    mov_speed_x = 0
            else:
                mov_speed_x = 0
                
            if posy > self.screen_size[1]-const.MOV_OFFSET:
                if (mov_speed_y == 0 or mov_speed_y < 0) and mov_speed_y < self.get_conf("max_mov_speed", int):
                    mov_speed_y -= int(self.get_conf("mov_speed", int)*deltat)
                else:
                    mov_speed_y = 0
            elif posy < const.MOV_OFFSET:
                if (mov_speed_y == 0 or mov_speed_y > 0) and mov_speed_y > -self.get_conf("max_mov_speed", int):
                    mov_speed_y += int(self.get_conf("mov_speed", int)*deltat)
                else:
                    mov_speed_y = 0
            else:
                mov_speed_y = 0
            
            self.world.move(mov_speed_x, mov_speed_y)
            self.world.zoom(1+zoom_speed)
            
            text = font.render("FPS : %d" % clock.get_fps(), 1, (255,0,0))
            self.world.update()
            
            screen.blit(self.background, (0,0))
            screen.blit(self.world.render(), (0,0))
            screen.blit(text, (10,10))
            
            pygame.display.update(Rect((0,0), self.screen_size))
        
        pygame.display.quit()
    
    def get_conf_file(self, path):
        self.conf = configparser.ConfigParser()
        self.conf.read(path)
    
    def get_conf(self, conf, type=str):
        return type(self.conf.get("CURRENT", conf))
        
    def propagate_trigger(self, event):
        self.world.propagate_trigger(event)

        
if __name__ == "__main__":
    client = Client()