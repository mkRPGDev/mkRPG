"""
Fichier de test pour l'entity_parser
"""

# -*- coding : utf-8 -*-

import unittest
import xml.etree.ElementTree as ET
from collections import OrderedDict
from os.path import abspath
import sys
sys.path.append(abspath("../../src"))
from parsing import entity_parser

ROOT = ET.parse("test_files/entity.xml").getroot().find("Entity")

class GetCharacteristicsTestCase(unittest.TestCase):
    def setUp(self):
        self.characteristics = OrderedDict([('picture', 88), ('x',
            15), ('y', 15), ('map', {'id': 'maiin'})])

    def test_get_characteristics(self):
        self.assertEqual(self.characteristics,
                        entity_parser.get_characteristics(ROOT.findall("Params")))

    def test_type_characteristic(self):
        self.assertEqual(int,
                        type(entity_parser.get_characteristics(ROOT.findall("Params"))['x']))

    def test_type_fail_characteristic(self):
        self.assertNotEqual(str,
                        type(entity_parser.get_characteristics(ROOT.findall("Params"))['x']))


class ParseEntityTestCase(unittest.TestCase):
    def setUp(self):
        self.parsed = OrderedDict([ ('name', 'moi'), ('type', 'Entity'),
            ('ident', 1), ('params', OrderedDict([('picture', 88), ('x', 15),
                ('y', 15), ('map', {'id': 'maiin'})]))])

    def test_parse_entity(self):
        self.assertEqual(self.parsed, entity_parser.parse_entity(ROOT))

if __name__ == "__main__":
    unittest.main()
