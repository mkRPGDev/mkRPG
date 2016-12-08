""" Define base elements for the GUI """

from enum import IntEnum

import pygame

from interface.cache import ImageCache

guiEvent = IntEnum('guiEvent', 'CLICK MOVE NONE')

class GUIElement(pygame.sprite.Sprite):
    """ Base class for GUI elements """

    elt_id = 0

    def __init__(self, style):
        """ sets style and id of the element """
        self.style = style
        self.my_id = self.elt_id
        self.elt_id += 1

        self.image = None
        self.rect = None

    def build(self):
        """ abstract method """
        pass

    def update(self, state):
        """ abstract method """
        pass

    def handle_event(self, event):
        """ abstract method """
        pass

class Button(GUIElement):
    """ Simple button """

    def __init__(self, style, action):
        """ Style should have the following attributes :
                size : int * int (if 0,0 size will fit the text)
                background_color : int * int * int * int (R,G,B and alpha)
                background_image : str (path of the file to use, if this
                                        attribute is specified and not empty,
                                        size and background_color will be
                                        ignored)
                text : string
                text_size : int
                text_font : string (name of the font file or None)
                text_align : string (should be "centered", "right" or "left")

                mover_background_color : int * int * int * int (color when mouse
                                                                is over)
                mover_background_image : str (path of the file to use, if this
                                              attribute is specified and not
                                              empty, mover_background_color will
                                              be ignored)

                click_background_color : int * int * int * int (color when
                                                                clicked)
                click_background_image : str (path of the file to use, if this
                                              attribute is specified and not
                                              empty, mover_background_color will
                                              be ignored)
            Action should be a function to call on click """
        super().__init__(style)

        self.state = "default"
        self.action = action
        self.build()

    def build(self):
        """ build all images for this element and cache them """

        self.font = pygame.font.Font(self.style['text_font'],
                                     self.style['text_size'])
        self.text = self.font.render(self.style['text'])

        for state in ["clicked", "mover", "default"]:
            if state == "clicked":
                back = "click_background"
            elif state == "mover":
                back = "mover_background"
            else:
                back = "background"

            if back+"_image" in self.style.keys():
                self.image = ImageCache.get_image(self.style[back+"_image"])
            else:
                if self.style['size'] == (0, 0):
                    sizex, sizey = self.text.get_size()
                    self.image = pygame.Surface((sizex+5, sizey+5),
                                                pygame.SRCALPHA)
                else:
                    self.image = pygame.Surface(self.style['size'],
                                                pygame.SRCALPHA)
                self.image = self.image.convert_alpha()
                self.image.fill(self.style[back+"_color"])

            self.rect = self.image.get_rect()

            textw, texth = self.text.get_size()
            buttw, butth = self.image.get_size()
            if self.style['text_align'] == "left":
                textx = (buttw-textw)*10//100
            elif self.style['text_align'] == "right":
                textx = (buttw-textw)*90//100
            else:
                textx = (buttw-textw)//2
            texty = (butth-texth)//2

            self.image.blit(self.text, (textx, texty))

            ImageCache.init_images([("gui_"+state+"_"+self.my_id, self.image)])

    def update(self, state="default"):
        """ switch between images depending on state """
        if state == "clicked" or state == "mover" or state == "default":
            self.image = ImageCache.get_image("gui_"+state+"_"+self.my_id)

    def handle_event(self, event):
        """ event should be guiEvent.CLICK, guiEvent.OVER or
            guiEvent.NONE """
        if event == guiEvent.CLICK and self.state != "clicked":
            self.update("clicked")
            self.state = "clicked"
            self.action()
        elif event == guiEvent.OVER and self.state != "mover":
            self.update("mover")
            self.state = "mover"
        elif event == guiEvent.NONE and self.state != "default":
            self.update()
            self.state = "default"
