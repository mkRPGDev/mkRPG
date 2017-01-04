""" Useful functions """
import pygame

def loadPng(name, scale=1):
        """ Load image and return image object"""
        image = pygame.image.load(name)
        if image.get_alpha is None:
                image = image.convert()
        else:
                image = image.convert_alpha()
        width, height = image.get_size()
        if scale != 1:
            image = pygame.transform.scale(image, (int(width*scale),
                                                   int(height*scale)))
        return image
