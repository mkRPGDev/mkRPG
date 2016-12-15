import pygame
from pygame.locals import Rect

#from time import sleep

#import shared.world as world

from interface.map import MapViewer
from interface.entity import Entity
from interface.utils import merge_rect_lists
#from interface.cache import ImageCache
#from interface.interface import Interface
import interface.const as const

class WorldViewer:

    def __init__(self, w):
        """ Initialize the pygame Interface """
        self.world = w
        self.screen_size = const.SCREEN_WIDTH, const.SCREEN_HEIGHT

        # Create current map by calling MapViewer
        self.current_map = MapViewer(self.world.currentMap, self.world)

        # Load entities
        self.entities = []
        for ent in w.entities:
            self.entities.append(Entity(ent,#(ent.x, ent.y),
                                   (self.current_map.cm_width,
                                    self.current_map.cm_height),
                                    ent.picture))
        self.perso = None

    def get_event(self):
        """ Return current pygame events stack """
        return pygame.event.get()

    def end(self):
        """ End the display """
        pygame.display.quit()

    def set_perso(self, perso):
        self.perso = perso

    def update(self):
        """ Update the view """
        # Update current_map and get the rectangles of modifications
        rect_arr = self.current_map.update()

        # Update entities and compute the rectangles of modifications
        charac_rects = []
        for ent in self.entities:
            rects = ent.update()#ent.x, ent.y)
            if rects is not None:
                for i in range(len(rects)):
                    rects[i] = rects[i].move(self.current_map.pos_offset)
                charac_rects = merge_rect_lists(charac_rects, rects)

        # Return the full list of rectangles of modifications
        return merge_rect_lists(charac_rects, rect_arr)

    def render(self):
        """ Return the new image to show on screen"""

        # Create empty Surface
        res = pygame.Surface(self.screen_size)
        res = res.convert_alpha()

        # Get the new map to show
        image = self.current_map.render()

        # Compute the right position offset for the map
        offsetx, offsety = self.current_map.chunk_pos

        res.blit(image, (offsetx, offsety))

        # Render every entitie and add it to the result
        for ent in self.entities:
            char_offset = (self.current_map.pos_offset[0]+ent.pos[0],
                           self.current_map.pos_offset[1]+ent.pos[1])
            res.blit(ent.render(), char_offset)

        return res

    def move(self, dx, dy):
        """ Move the current map by dx,dy, return the rectangle of
            modifications"""
        if dx != 0 or dy != 0:
            self.current_map.move(dx, dy)

            # If the map has effectively been moved, the rectangle of
            # modifications is the whole screen
            return [Rect((0,0),self.screen_size)]

    def propagate_trigger(self, event):
        """ Propagate event to next objects in the arborescence, here its the
            current map """
        return self.current_map.propagate_trigger(event)
