""" Will be removed
    This file is used to test and debug the gui"""

import sys
import pygame
from pygame.locals import QUIT

from gui_elements import Container, TextField, ScrollableTextField, Button

pygame.display.init()
pygame.font.init()
screen = pygame.display.set_mode((800,600))
clock = pygame.time.Clock()

background = pygame.Surface((800,600))
background = background.convert()
background.fill((0, 0, 0))

container = Container([],
                      {
                        'size':(0,0),
                        'background_color':(255,255,255,255)
                      })
container.move(20,20)

button = Button("Click me!", lambda: print("Click"),
                {
                    'size':(0,0),
                    'background_color':(255,0,0,255),
                    'text_size':50,
                    'text_color':(0,0,0,0),
                    'text_font':None,
                    'text_align':'centered',
                    'text_interline':2,
                    'mover_background_color':(0,255,0,255),
                    'click_background_color':(0,0,255,255),
                })
button.move(50,50)

textfield = TextField("This is a loooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "oooooooooooooooooooooooooooooooooooooooooooooog text!",
                      {
                        'size':(0,0),
                        'text_size':20,
                        'text_color':(255,255,0,255),
                        'text_font':None,
                        'text_align':'left',
                        'text_interline':2
                      })
textfield.move(350,200)

scrolltextfield = ScrollableTextField("This is a loooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "ooooooooooooooooooooooooooooooooooooooooooooooooooo\n"+\
                      "oooooooooooooooooooooooooooooooooooooooooooooog\n"+\
                      "scrollable text!",
                      {
                        'size':(450,100),
                        'text_size':20,
                        'text_color':(255,0,255,255),
                        'text_font':None,
                        'text_align':'left',
                        'text_interline':2,
                        'scroll_step':10
                      })
scrolltextfield.move(0,100)

container.add(button)
container.add(textfield)
container.add(scrolltextfield)

while True:
    clock.tick(60)

    for event in pygame.event.get():
        if event.type == QUIT:
            print("KILL")
            pygame.display.quit()
            exit(0)
        else:
            container.handle_event(event)

    screen.blit(background, (0,0))
    screen.blit(container.render(), container.pos)

    container.update()
    pygame.display.flip()


pygame.display.quit()
