import unittest
from sys import path
from os.path import abspath
path.append(abspath("src/"))
print(path)
from shared.world import Cell, Map, ObjectType

class MapTestCase(unittest.TestCase):
    def setUp(self):
        """ creation of a map """
        # 0 _____
        # 1 _____
        # 2 __O__
        # 3 ___O_
        # 4 _____
        self.map = Map()
        self.map.params['width'] = 5
        self.map.params['height']= 5
        cd = ObjectType(typ=Cell)
        cd.params['visible'] = False
        self.map.params['defaultCell'] = cd
        c1 = Cell(); c1.params['visible'] = True
        c1.params['x'] = 2; c1.params['y'] = 2
        self.map.cells.append(c1)
        c2 = Cell(); c2.params['visible'] = True
        c2.params['x'] = 3; c2.params['y'] = 3
        self.map.cells.append(c1)
        self.map.fill()

    def test_fill(self):
        self.assertIsInstance(self.map.cellsGrid[0][0], Cell)
        self.assertEqual(self.map.cellsGrid[3][0].x, 3)

    def test_dist(self):
        self.assertEqual(self.map.dist(self.map.cellsGrid[0][1],
                                       self.map.cellsGrid[2][3]), 4)

    def test_fromPos(self):
        """ Sur une zone de taille 2 on vérifie que le nombre y est
            et que les cases sont rangées par distance """
        # _____
        # __x__
        # _xxx_
        # xxOxx
        # _xxx_
        lis = list(self.map.fromPos(self.map.cellsGrid[2][3], 2))
        self.assertEqual(len(lis), 11) # comptez les x
        dlis = list(map(lambda c:self.map.dist(self.map.cellsGrid[2][3], c), lis))
        self.assertEqual(sorted(dlis), dlis)

    def test_lov(self):
        self.assertTrue(self.map.lov(self.map.cellsGrid[0][1],
                                     self.map.cellsGrid[3][4]))
        self.assertTrue(self.map.lov(self.map.cellsGrid[1][1],
                                     self.map.cellsGrid[1][1]))
        self.assertFalse(self.map.lov(self.map.cellsGrid[0][2],
                                      self.map.cellsGrid[4][2]))

if __name__ == '__main__':
    unittest.main()
