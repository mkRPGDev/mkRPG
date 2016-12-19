"""
Fichier de test pour utils.py (squelette entièrement écrit)
"""
# -*- coding : utf-8 -*-

import unittest
from src.interface import utils

class AddToRectListTestCase(unittest.TestCase):
    def setUp(self):
        #Faire une liste de Rect et non d'entier ?
        self.list = [1,2]
        #Rect((x,y,l,w))

    """ On vérifie que si on ajoute rien, la liste est inchangée"""
    def test_rect_empty(self):
        self.assertEqual(self.list,utils.add_to_rect_list(self.list,None),
                         'add None to rect_list')

    """ Tous les élements de la liste se retrouvent dans la liste crée """

    """ L'élément nouveau se retrouve dans la liste crée """

    """ La longueur de la liste retournée est la même que celle en entrée
    augmentée de un """

class MergeRectListTestCase(unittest.TestCase):

    """ Cas de bases avec des listes vides pour liste1 et liste2 """

    """ Cas de bases avec une liste vide pour liste1 """

    """ Cas de bases avec une liste vide pour liste2 """

    """ La longueur de la liste retournée est la somme de celles en entrée """

    """ Tous les élements de deux listes se retrouvent dans la liste crée """


# class CellPointTransformation(unittest.TestCase):
#     def setUp(self):
#
#
#
# def testCoord(x,y,angx,angy):
#     # fonction de test des changement de coordonnées.
#     angy += 900
#     deg_to_rad = lambda a : a*pi/1800
#     (a,b) = point_to_cell(x,y,42,42,64, deg_to_rad(angx),deg_to_rad(angy))
#     (c,d) = cell_to_point(x,y,42,42,64, deg_to_rad(angx),deg_to_rad(angy))
#     print("pt -> cl -> pt", (x,y), cell_to_point(a,b,42,42,64, deg_to_rad(angx),deg_to_rad(angy)))
#     print("cl -> pt -> cl", (x,y), point_to_cell(c,d,42,42,64, deg_to_rad(angx),deg_to_rad(angy)))


if __name__ == '__main__':
    unittest.main()
