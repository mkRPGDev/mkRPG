""" Define base elements for the GUI """

from enum import IntEnum

import pygame
from pygame.locals import Rect, MOUSEBUTTONUP, MOUSEBUTTONDOWN, MOUSEMOTION

guiEvent = IntEnum('guiEvent', 'CLICK MOVE OVER NONE SCROLL_UP SCROLL_DOWN')

class GUIElement(pygame.sprite.Sprite):
    """ Base class for GUI elements """

    elt_id = 0

    def __init__(self, style):
        """ sets style and id of the element """
        super().__init__()
        self.style = style
        self.my_id = self.elt_id
        self.elt_id += 1

        self.image = None
        self.rect = None

    def build(self):
        """ abstract method """
        pass

    def update(self, state="default"):
        """ abstract method """
        pass

    def handle_event(self, event):
        """ abstract method """
        pass

    def set_position(self, x, y):
        self.rect = Rect((x,y), self.rect.size)

    def move(self, dx, dy):
        self.rect.move_ip(dx, dy)

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
                background_image : pygame.Surface (to use
                                        Note that unlike the Button class, the
                                        container class requires a
                                        background_color attribute even if
                                        this attribute is specified)
                background_pattern : str (should be "unique" or "repeat", only
                                          necessary when background_image is
                                          used)
        """
        super().__init__(elements)

        self.style = style
        self.pos = (0,0)

        self.build()
        self.need_build = False

    def build(self):
        """ build the background of the container """
        if self.style['size'] == (0, 0):
            rect = Rect(0, 0, 0, 0)
            for sprite in self.sprites():
                rect.union_ip(sprite.rect)
            size = rect.size
        else:
            size = self.style['size']

        self.background = pygame.Surface(size, pygame.SRCALPHA)
        self.background = self.background.convert_alpha()
        self.background.fill(self.style['background_color'])

        if 'background_image' in self.style.keys() and\
            self.style['background_image'] != "":
            image = self.style['background_image']
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
                print("[WARNING] Container %i : Unrecognized value %s for "+\
                      "background_pattern attribute, assuming it is unique" %\
                      (self.my_id, self.style['background_pattern']))
                self.background.blit(image, (0, 0))

    def add(self, *sprites):
        """ add GUIElement sub class instances to the container """
        for sprite in sprites:
            super().add(sprite)
        self.need_build = True

    def remove(self, *sprites):
        """ remove GUIElement sub class instances to the container """
        super().remove(sprites)
        self.need_build = True

    def empty(self):
        """ remove everything from the container """
        super().empty()
        self.need_build = True

    def draw(self, surface):
        """ draw all elements (except background) on surface """
        if self.need_build:
            self.build()
            self.need_build = False

        surface.blit(self.background, (0,0))
        super().draw(surface)

    def render(self):
        """ draw all the elements over the background and return it """
        result = self.background.copy()
        self.draw(result)

        return result

    def handle_event(self, event):
        unmodified_sprites = self.sprites()
        if event.type == MOUSEBUTTONUP or event.type == MOUSEBUTTONDOWN or\
           event.type == MOUSEMOTION:
            rel_pos = (event.pos[0]-self.pos[0], event.pos[1]-self.pos[1])
            for sprite in self.sprites():
                if sprite.rect.collidepoint(rel_pos):
                    if sprite in unmodified_sprites:
                        unmodified_sprites.remove(sprite)

                    sprite.handle_event(guiEvent.OVER)
                    if event.type == MOUSEBUTTONUP or\
                       event.type == MOUSEBUTTONDOWN:
                        if event.button == 1:
                            # Click
                            sprite.handle_event(guiEvent.CLICK)
                        elif event.button == 4:
                            # Wheel up
                            sprite.handle_event(guiEvent.SCROLL_UP)
                        elif event.button == 5:
                            # Wheel down
                            sprite.handle_event(guiEvent.SCROLL_DOWN)
        for sprite in unmodified_sprites:
            sprite.handle_event(guiEvent.NONE)

    def move(self, dx, dy):
        self.pos = (self.pos[0]+dx, self.pos[1]+dy)

    def set_position(self, x, y):
        self.pos = (x, y)

class TextField(GUIElement):
    """ Simple text field """

    def __init__(self, text, style):
        """ Style should have the following attributes:
                size : int * int (if 0,0 the area will fit the text)
                text_size : int
                text_color : int * int * int * int (R,G,B and alpha values)
                text_font : string (name of the font file or None)
                text_align : string (should be "centered", "right" or "left")

            Note : Rendered text won't fit automatically on the given size if
            it's too big, one should put endlines where needed and a correct
            size
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
            text_lines.append(self.font.render(line, 1,
                                               self.style['text_color']))
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
                print("[WARNING] Button %i : Unrecognized value %s for "+\
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

class ScrollableTextField(GUIElement):
    """ Scrollable text field (only on y axis) """

    def __init__(self, text, style):
        """ Style should have the following attributes:
                size : int * int (required)
                text_size : int
                text_color : int * int * int * int (R,G,B and alpha values)
                text_font : string (name of the font file or None)
                text_align : string (should be "centered", "right" or "left")

                scroll_step : int (pixels to scroll at each scroll step)

            Note : Rendered text won't fit automatically on the given size if
            it's too big, one should put endlines where needed
        """
        super().__init__(style)
        self.text = text
        self.textField = None # TextField that will be scrolled
        self.tf_ypos = 0 # relative position in the wrapper
        self.wrapper = None # pygame.Surface that contain the TextField object
                            # to be scrolled

        self.build()
        self.update()

    def build(self):
        tf_style = self.style.copy()
        tf_style['size'] = (0,0)
        self.textfield = TextField(self.text, tf_style)
        self.tf_ypos = 0

        self.rect = Rect((0,0), self.style['size'])

    def update(self):
        self.wrapper = pygame.Surface(self.style['size'], pygame.SRCALPHA)
        self.wrapper = self.wrapper.convert_alpha()

        self.wrapper.blit(self.textfield.image, (0,self.tf_ypos))
        self.image = self.wrapper

    def scroll(self, dz):
        self.tf_ypos += dz

    def handle_event(self, event):
        if event == guiEvent.SCROLL_UP:
            if self.tf_ypos >= -self.textfield.rect.size[1] +\
                               2*self.style['text_size']:
                self.scroll(-self.style['scroll_step'])
        elif event == guiEvent.SCROLL_DOWN:
            if self.tf_ypos < 0:
                self.scroll(self.style['scroll_step'])


class Button(TextField):
    """ Simple button """

    def __init__(self, text, action, style):
        """ Style should have the following attributes:
                size : int * int (if 0,0 size will fit the text)
                background_color : int * int * int * int (R,G,B and alpha)
                background_image : pygame.Surface (to use, if this
                                        attribute is specified and not empty,
                                        size and background_color will be
                                        ignored)
                text_size : int
                text_color : int * int * int * int (R,G,B and alpha values)
                text_font : string (name of the font file or None)
                text_align : string (should be "centered", "right" or "left")
                text_interline : int (pixels between two lines)

                mover_background_color : int * int * int * int (color when mouse
                                                                is over)
                mover_background_image : pygame.Surface (to use, if this
                                        attribute is specified and not empty,
                                        size and background_color will be
                                        ignored)

                click_background_color : int * int * int * int (color when
                                                                clicked)
                click_background_image : pygame.Surface (to use, if this
                                        attribute is specified and not empty,
                                        size and background_color will be
                                        ignored)
            Action should be a function to call on click

            Note : Rendered text won't fit automatically on the given size if
            it's too big, one should put endlines where needed
        """
        self.state = "default"
        self.action = action
        self.cache = {}

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
                self.image = self.style[back+"_image"]
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

            self.cache["gui_"+str(self.my_id)+"_"+state] = self.image

    def update(self, state=None):
        """ switch between images depending on state """
        if state is None:
            state == self.state

        if state == "clicked" or state == "mover" or state == "default":
            self.image = self.cache["gui_"+str(self.my_id)+"_"+state]

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
            self.update("default")
            self.state = "default"
