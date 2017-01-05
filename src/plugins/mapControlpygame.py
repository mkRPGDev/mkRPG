"""
    This plugin adds buttons to change the viewpoint of the camera. Pygame part
"""

import pygame
from pygame.locals import Rect

from plugins.pluginpygame import PygamePlugin
from interface.gui.gui_elements import Container, Button
from interface.utils import loadPng
from interface.const import FLAT_STEP, ROT_STEP

class MapcontrolSprite(PygamePlugin):
    """ MapControl plugin """
    def __init__(self, logic):
        super().__init__(logic)

        pygame.init()
        pygame.display.set_mode((200,200))

        # Load arrows images for the onscreen pad
        arrows = {}
        arrows['up'] = loadPng('../content/imgs/arrow_up.png')
        arrows['down'] = loadPng('../content/imgs/arrow_down.png')
        arrows['left'] = loadPng('../content/imgs/arrow_left.png')
        arrows['right'] = loadPng('../content/imgs/arrow_right.png')

        # Load img
        #img = loadPng('../content/imgs/compass.png')
        # FIXME t'as oublié de commit l'image...

        # Create the pad to put on screen
        self.sprite = Container([],
                                {
                                    'size':(0,0),
                                    'background_color':(0,0,0,0)
                                })

        button_up = Button("",
                           lambda: logic.flatten(FLAT_STEP),
                           {
                               'size':(0,0),
                               'background_image': arrows['up'],
                               'mover_background_image': arrows['up'],
                               'click_background_image': arrows['up'],
                               'text_font': None,
                               'text_color': (0,0,0,0),
                               'text_size': 0,
                               'text_interline': 0,
                               'text_align' : "centered"
                           })

        button_down = Button("",
                           lambda: logic.flatten(-FLAT_STEP),
                           {
                               'size':(0,0),
                               'background_image': arrows['down'],
                               'mover_background_image': arrows['down'],
                               'click_background_image': arrows['down'],
                               'text_font': None,
                               'text_color': (0,0,0,0),
                               'text_size': 0,
                               'text_interline': 0,
                               'text_align' : "centered"
                           })

        button_left = Button("",
                           lambda: logic.rotate(ROT_STEP),
                           {
                               'size':(0,0),
                               'background_image': arrows['left'],
                               'mover_background_image': arrows['left'],
                               'click_background_image': arrows['left'],
                               'text_font': None,
                               'text_color': (0,0,0,0),
                               'text_size': 0,
                               'text_interline': 0,
                               'text_align' : "centered"
                           })

        button_right = Button("",
                           lambda: logic.rotate(-ROT_STEP),
                           {
                               'size':(0,0),
                               'background_image': arrows['right'],
                               'mover_background_image': arrows['right'],
                               'click_background_image': arrows['right'],
                               'text_font': None,
                               'text_color': (0,0,0,0),
                               'text_size': 0,
                               'text_interline': 0,
                               'text_align' : "centered"
                           })

#        img_cont = Button("",
#                          lambda: padd,
#                          {
#                              'size':(0,0),
#                              'background_image': None,#img,
#                              'mover_background_image': None,#img,
#                              'click_background_image': None,#img,
#                              'text_font': None,
#                              'text_color': (0,0,0,0),
#                              'text_size': 0,
#                              'text_interline': 0,
#                              'text_align' : "centered"
#                          })

        but_w, but_h = arrows['up'].get_size()

        button_up.set_position(but_w, 0)
        button_left.set_position(0, but_h)
        button_down.set_position(but_w, 2*but_h)
        button_right.set_position(2*but_w, but_h)
        #img_cont.set_position(but_w, but_h)

        self.sprite.add(button_up, button_down, button_left, button_right)
#        ,
#                        img_cont)
        self.sprite.move(30, 30)

    def draw(self):
        """ actually draw the pad and update sprite attributes """
        self.image = self.sprite.render()
        self.rect = Rect(self.sprite.pos, self.image.get_size())

    def handleEvent(self, event):
        """ handle event by propagating it to the container """
        self.sprite.handle_event(event)
