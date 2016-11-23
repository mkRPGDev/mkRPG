import pygame
from pygame.locals import Rect

from cache import ImageCache
import const
from utils import add_to_rect_list, cell_to_point

class Entity(pygame.Surface):
    def __init__(self, pos, map_size, picture):
        self.x, self.y = pos
        self.map_width, self.map_height = map_size
        self.picture = picture
        self.image = ImageCache.get_image(const.IMG_SET[self.picture])
        self.pos = self.get_ent_pos()

    def render(self):
        return self.image

    def get_ent_pos(self):
        posx, posy = cell_to_point(self.x+0.5,
                                   self.y+0.5,
                                   self.map_width,
                                   self.map_height,
                                   const.CELL_WIDTH,
                                   const.ANGLE_X_R,
                                   const.ANGLE_Y_R)
        width,height = self.image.get_size()
        posx -= width/2
        posy -= height/2
        return int(posx),int(posy)

    def update(self, newx, newy):
        self.x, self.y = newx, newy
        lastpos = self.pos
        self.pos = self.get_ent_pos()
        lastrect = Rect(lastpos, self.image.get_size())
        rect = Rect(self.pos, self.image.get_size())
        return add_to_rect_list([lastrect], rect)
