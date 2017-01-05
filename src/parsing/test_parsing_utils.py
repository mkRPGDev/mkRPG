"""
Fichier de test pour l'entity_parser
"""

# -*- coding : utf-8 -*-

import unittest
import xml.etree.ElementTree as ET
from collections import OrderedDict
from parsing import parsing_utils

ROOT = ET.parse("parsing/test_files/entity.xml").getroot().find("Entity")

class FormatTypeTestCase(unittest.TestCase):
    def setUp(self):
        self.integer = 1
        self.float = 1.1
        self.string = "33c3"

    def test_format_type_int(self):
        self.assertEqual(1, parsing_utils.format_type("1"))

    def test_format_type_float(self):
        self.assertEqual(1.1, parsing_utils.format_type("1.1"))

    def test_format_type_string(self):
        self.assertEqual("33c3", parsing_utils.format_type("33c3"))



if __name__ == "__main__":
    unittest.main()
