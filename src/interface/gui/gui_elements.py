""" Define base elements for the GUI """

from enum import IntEnum

import pygame
from pygame.locals import Rect

from interface.cache import ImageCache
from interface.utils import debug

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

class Container(pygame.sprite.Group):
    """ Simple container for other elements """

    def __init__(self, elements, style):
        """ Elements is a list of GUIElement sub-classes instances and their
            position relatively to the top left corner of the container. Each
            element will be blit accordingly to its Rect attribute (position
            is relative to the container top left corner)
            Style should have the following attributes:
                size : int * int (if 0,0 the area will fit)
                background_color : int * int * int * int (R,G,B and alpha)
                background_image : str (path of the file to use, not necessary.
                                        Note that unlike the Button class, the
                                        container class requires a
                                        background_color attribute even if
                                        this attribute is specified)
                background_pattern : str (should be "unique" or "repeat", only
                                          necessary when background_image is
                                          used)
        """
        super().__init__(elements)
        self.build()
        self.need_build = False

    def build(self):
        """ build the background of the container """
        if self.style['size'] == (0, 0):
            rect = Rect(0, 0, 0, 0)
            for sprite in self.sprits():
                rect.union_ip(sprite.rect)
            size = rect.size()
        else:
            size = self.style['size']

        self.background = pygame.Surface(size, pygame.SRCALPHA)
        self.background = self.background.convert_alpha()
        self.background.fill(self.style['background_color'])

        if 'background_image' in self.style.keys() and\
            self.style['background_image'] != "":
            image = ImageCache.get_image(self.style['background_image'])
            if self.style['background_pattern'] == "unique":
                self.background.blit(image, (0, 0))
            elif self.style['background_pattern'] == "repeat":
                size_image = image.get_size()
                repx = size[0] // size_image[0] + 1
                repy = size[1] // size_image[1] + 1
                for __x in repx:
                    for __y in repy:
                        self.image.blit(image, (__x*size_image[0],
                                                __y*size_image[1]))
            else:
                debug("WARNING", "Container %i : Unrecognized value %s for "+\
                      "background_pattern attribute, assuming it is unique" %\
                      (self.my_id, self.style['background_pattern']))

        def add(self, *sprites):
            """ add GUIElement sub class instances to the container """
            super().add(sprites)
            self.need_build = True

        def remove(self, *sprites):
            """ remove GUIElement sub class instances to the container """
            super().remove(sprites)
            self.need_build = True

        def empty(self):
            """ remove everything from the container """
            super().empty()
            self.need_build = True

        def render(self):
            """ draw all the elements over the background and return it """
            if self.need_build:
                self.build()
                self.need_build = False

            result = self.background.copy()
            self.draw(result)

            return result



class TextField(GUIElement):
    """ Simple text field """

    def __init__(self, text, style):
        """ Style should have the following attributes:
                size : int * int (if 0,0 the area will fit the text)
                text_size : int
                text_color : int * int * int * int (R,G,B and alpha values)
                text_font : string (name of the font file or None)
                text_align : string (should be "centered", "right" or "left")
        """
        super().__init__(style)
        self.text = text

        self.font = pygame.font.Font(self.style['text_font'],
                                     self.style['text_size'])
        self.textRender = None

        self.build()

    def build_text(self, text):
        """ build a new button with the new text and the new size if specified
        """

        text_lines = []
        max_width = 0
        for line in text.split("\n"):
            text_lines.append(self.font.render(line))
            line_width = text_lines[-1].get_size()[0]
            if line_width > max_width:
                max_width = line_width

        text_height = sum([line.get_size()[1] for line in text_lines])+\
                      (len(text_lines)-1)*self.style['text_interline']
        text_width = max_width

        self.textRender = pygame.Surface((text_width, text_height),
                                         pygame.SRCALPHA)
        self.textRender = self.textRender.convert_alpha()

        currenty = 0
        for line in text_lines:
            line_width, line_height = line.get_size()

            if self.style['text_align'] == "centered":
                linex = (text_width-line_width)//2
            elif self.style['text_align'] == "left":
                linex = 0
            elif self.style['text_align'] == "right":
                linex = (text_width-line_width)
            else:
                debug("WARNING", "Button %i : Unrecognized value %s for "+\
                      "text_align attribute, assuming it is centered" %\
                      (self.my_id, self.style['text_align']))
                self.style['text_align'] = "centered"
                linex = (text_width-line_width)//2

            liney = currenty
            currenty += line_height + self.style['text_interline']

            self.textRender.blit(line, (linex, liney))

    def build(self):
        """ build the text field and cache it """

        self.build_text(self.text)

        textw, texth = self.textRender.get_size()

        if self.style['size'] == (0, 0):
            size = (textw + 10, texth + 10)
        else:
            size = self.style['size']

        self.image = pygame.Surface(size, pygame.SRCALPHA)
        self.image = self.image.convert_alpha()
        self.rect = self.image.get_rect()

        # At this point, self.style['text_align'] can only be left, right or
        # centered thanks to self.build_text
        if self.style['text_align'] == "left":
            textx = (size[0]-textw)//10
        elif self.style['text_align'] == "right":
            textx = (size[0]-textw)*9//10
        else:
            textx = (size[0]-textw)//2

        texty = (size[1]-texth)//2

        self.image.blit(self.textRender, (textx, texty))

        return "gui_"+self.my_id

class Button(TextField):
    """ Simple button """

    def __init__(self, text, action, style):
        """ Style should have the following attributes:
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
                text_interline : int (pixels between two lines)

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
            Action should be a function to call on click

            Note : Rendered text won't fit automatically on the given size if
            it's too big, one should put endlines where needed
        """
        self.state = "default"
        self.action = action

        super().__init__(text, style)

    def build(self):
        """ build all images for this element and cache them """

        # We only render the text once, it's the same for all states of the
        # button. Rendered text is stored in self.textRender
        self.build_text(self.text)

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
                    sizex, sizey = self.textRender.get_size()
                    size = (sizex+10, sizey+10)
                else:
                    size = self.style['size']

                self.image = pygame.Surface(size,
                                            pygame.SRCALPHA)
                self.image = self.image.convert_alpha()
                self.image.fill(self.style[back+"_color"])

            self.rect = self.image.get_rect()

            textw, texth = self.textRender.get_size()
            buttw, butth = self.image.get_size()
            if self.style['text_align'] == "left":
                textx = (buttw-textw)*1//10
            elif self.style['text_align'] == "right":
                textx = (buttw-textw)*9//10
            else:
                textx = (buttw-textw)//2
            texty = (butth-texth)//2

            self.image.blit(self.textRender, (textx, texty))

            ImageCache.init_images([("gui_"+self.my_id+"_"+state, self.image)])

        return "gui_"+self.my_id

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
