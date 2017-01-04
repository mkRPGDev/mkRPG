# -*- coding: utf-8 -*-

"""
    This module contains auxiliary functions used in the project.
"""

from math import sqrt

import pygame

def loadPng(filename, scale=1):
    """
        Loads an image into a pygame.image object

        @fileparam filename the filename of the image to load
        @param scale the multiplicative factor to be applied to the image size
        @returns the pygame.image object containing the image at the right scale

        @todo Add an exception in case the loading fails (bad name for instance)
    """
    image = pygame.image.load(filename)
    image = image.convert() if image.get_alpha is None else image.convert_alpha()
    if scale != 1:
        width, height = image.get_size()
        image = pygame.transform.scale(image, (int(width*scale),
                                               int(height*scale)))
    return image

def add_to_rect_list(rect_list, rect):
    """
        Adds a new pygame.Rect object to a list, ignoring included objects and filtering None.
        The input data is not modified.

        @param rect_list a Rect objects list where the new object should be added
        @param rect the Rect object to be added to the list
        @returns a new list containing the new rect, with no None and no included objects
    """
    if rect_list is None:
        return [rect] if rect is not None else []
    ans = [x for x in rect_list if x is not None]
    if rect is None:
        return ans

    add_rect = True
    for rect_in_ans in ans:
        if rect_in_ans.contains(rect):
            add_rect = False
        elif rect.contains(rect_in_ans):
            ans.remove(rect_in_ans)
    if add_rect:
        ans.append(rect)
    return ans

def merge_rect_lists(rect_list1, rect_list2):
    """
        Merges two lists of Pygame.Rect objects, ignoring included objects and filtering None.
        The input data is not modified.

        @param list1 the first Rect objects list to be merged
        @param list2 the second Rect objects list to be merged
        @returns a new list containing the elements of the two lists, with no\
            None and no included objects
    """
    if rect_list1 is None:
        return [x for x in rect_list2 if x is not None] \
            if rect_list2 is not None else []
    ans = [x for x in rect_list1 if x is not None]
    if rect_list2 is None:
        return ans

    for rect_in_list2 in rect_list2:
        add_rect = True
        for rect_in_ans in ans:
            if rect_in_ans.contains(rect_in_list2):
                add_rect = False
            elif rect_in_list2.contains(rect_in_ans):
                ans.remove(rect_in_ans)
        if add_rect:
            ans.append(rect_in_list2)

    return ans

def submatrix(matrix, row_start, row_end, column_start, column_end):
    """
        Returns the sub matrix defined by
        matrix[row_start:row_end][column_start:column_end]

        @param matrix the input matrix
        @param row_start the starting row index
        @param row_end the ending row index
        @param column_start the starting column index
        @param column_end the starting column index
        @returns the submatrix defined by the input indexes

        @warning The row_end-th row and the column_end-th column are excluded
    """
    return [row[column_start:column_end] for row in matrix[row_start:row_end]]
