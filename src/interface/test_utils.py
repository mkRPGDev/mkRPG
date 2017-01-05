# -*- coding : utf-8 -*-

"""
    Test file for interface/utils.py
    @see utils.py
"""

import unittest
from interface import utils

from pygame import Rect

class AddToRectListTestCase(unittest.TestCase):
    """ Test class for add_to_rect function """

    def setUp(self):

        self.rect_list = [Rect(1, 2, 3, 4)]
        self.rect = Rect(1, 2, 3, 4)

        self.saved_rect = self.rect
        self.saved_rect_list = self.rect_list

        self.new_list = utils.add_to_rect_list(self.rect_list, self.rect)

    def test_former_elements(self):
        """ Every rect in the former list is contained in exactly one rect in the new one """
        for former_element in self.rect_list:
            self.assertEqual(
                [x.contains(former_element) for x in self.new_list].count(True), 1
            )

    def test_new_element(self):
        """ The added element is contained in exactly one rect of the new one """
        self.assertEqual([x.contains(self.rect) for x in self.new_list].count(True), 1)

    def test_length(self):
        """ The returned list length is at most the former one plus one """
        self.assertLess(len(self.new_list), len(self.rect_list) + 1)

    def test_no_useless(self):
        """
            The returned list does not contain any useless (thus unwanted)
            element, i.e. an object not present in the former list
        """
        for new_element in self.new_list:
            self.assertTrue(any(x.contains(new_element) for x in self.rect_list))

    def test_remaining_none(self):
        """ There are no None in the returned list """
        self.assertNotIn(None, self.new_list)

    def test_data_not_modified(self):
        """ The input data was not modified """
        self.assertEqual(self.rect_list, self.saved_rect_list)
        self.assertEqual(self.rect, self.saved_rect)


class MergeRectListTestCase(unittest.TestCase):
    """ Test class for merge_rect_lists function """

    def setUp(self):
        self.list1 = [Rect(1, 2, 3, 4)]
        self.list2 = [Rect(1, 2, 3, 4)]

        self.saved_list1 = self.list1
        self.saved_list2 = self.list2

        self.new_list = utils.merge_rect_lists(self.list1, self.list2)

    def test_former_elements(self):
        """
            Every rect in the former lists is contained in exactly one rect in
            the new one
        """
        for former_element in self.list1 + self.list2:
            self.assertEqual(
                [x.contains(former_element) for x in self.new_list].count(True), 1
            )

    def test_length(self):
        """ The returned list length is at most the former one plus one """
        self.assertLess(len(self.new_list), len(self.list1) + len(self.list2))

    def test_no_useless(self):
        """
            The returned list does not contain any useless (thus unwanted)
            element, i.e. an object not present in the former list
        """
        for new_element in self.new_list:
            self.assertTrue(any(x.contains(new_element) for x in self.list1 + self.list2))

    def test_remaining_none(self):
        """ There are no None in the returned list """
        self.assertNotIn(None, self.new_list)

    def test_data_not_modified(self):
        """ The input data was not modified """
        self.assertEqual(self.list1, self.saved_list1)
        self.assertEqual(self.list2, self.saved_list2)

if __name__ == '__main__':
    unittest.main()
