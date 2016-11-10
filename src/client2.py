# -*- coding: utf-8 -*-

import configparser

import pygame
from pygame.locals import QUIT, K_ESCAPE, KEYUP, MOUSEBUTTONDOWN, MOUSEBUTTONUP
from pygame.locals import Rect

from printWorld import PrintWorld
from utils import add_to_rect_list
import const

class Client():
    
    def __init__(self):
        self.get_conf_file("client_conf.ini")
        
        pygame.display.init()
        self.screen_size = const.SCREEN_WIDTH, const.SCREEN_HEIGHT
        screen = pygame.display.set_mode(self.screen_size)
        
        pygame.font.init()
        font = pygame.font.Font(None, 18)
        
        self.world = PrintWorld()
        
        self.background = pygame.Surface(self.screen_size)
        self.background = self.background.convert()
        self.background.fill((0, 0, 0))
        
        clock = pygame.time.Clock()
        
        mov_speed_x = 0
        mov_speed_y = 0
        
        refresh_counter = self.frame_counter(60)

        while(1):
            deltat = clock.tick(60)/1000
            zoom_speed = 0
            
            for event in pygame.event.get():
                if event.type == QUIT:
                    exit(0)
                elif event.type == KEYUP and event.key == K_ESCAPE:
                    exit(0)
                elif event.type == MOUSEBUTTONDOWN and event.button == 4:
                    zoom_speed += const.ZOOM_STEP
                elif event.type == MOUSEBUTTONDOWN and event.button == 5:
                    zoom_speed -= const.ZOOM_STEP
                elif event.type == MOUSEBUTTONUP and event.button == 1:
                    self.propagate_trigger(event)
            
            mov_speed_x, mov_speed_y, rect_list = self.update_view(pygame.mouse.get_pos(), 
                                                        mov_speed_x, 
                                                        mov_speed_y,
                                                        deltat)
            
            # rect_list = self.world.zoom(zoom_speed) if rect_list is None else rect_list
            
            text = font.render("FPS : %d" % clock.get_fps(), 1, (255,0,0))
            
            new_rect_list = self.world.update()
            
            if rect_list is None:
                rect_list = new_rect_list
                
            rect_list = add_to_rect_list(rect_list, text.get_rect().move(10,10))
            
            image = self.world.render()
            
            screen.blit(self.background, (0,0))
            screen.blit(image, (0,0))
            screen.blit(text, (10,10))
            
            if next(refresh_counter) : pygame.display.flip()
            else : pygame.display.update(rect_list)
        
        pygame.display.quit()
    
    def frame_counter(self, n):
        i = n
        while 1:
            if i == n:
                i = 0
                yield True
            else:
                i += 1
                yield False
        
    def update_view(self, mouse_pos, mov_speed_x, mov_speed_y, deltat):
        posx,posy = mouse_pos
    
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

        rect = self.world.move(mov_speed_x, mov_speed_y)
        
        return mov_speed_x, mov_speed_y, rect
    
    def get_conf_file(self, path):
        self.conf = configparser.ConfigParser()
        self.conf.read(path)
    
    def get_conf(self, conf, type=str):
        return type(self.conf.get("CURRENT", conf))
        
    def propagate_trigger(self, event):
        message = self.world.propagate_trigger(event)
        if message != "":
            self.send_msg(message)
    
    def send_msg(self, message):
        print(message)
        self.get_msg(message+" Helyder")
    
    def get_msg(self, message):
        parse = message.split(" ")
        if parse[0] == "MOVE":
            endpos = tuple([int(s) for s in parse[1].split(",")])
            self.world.move_char(parse[2], endpos)
    
    def init_cache(self):
        ImageCache.init_images(const.BG_IMGSET.values())
        ImageCache.init_images(const.GRID_IMGSET.values())

        
if __name__ == "__main__":
    client = Client()