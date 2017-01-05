"""
Fichier de test pour l'action_parser
"""

# -*- coding : utf-8 -*-

import unittest
import xml.etree.ElementTree as ET
from collections import OrderedDict
import os
from sys import path
from os.path import abspath
path.append(abspath("src/"))
from parsing import interactions_parser

ROOT = ET.parse("parsing/test_files/interactions.xml").getroot().find("Interaction")

class ParseInteractionTestCase(unittest.TestCase):
    def setUp(self):
        self.parsed = OrderedDict([('key', 122), ('target', 'perso'), ('event',
            'moveup')])
    def test_parse_interaction(self):
        self.assertEqual(self.parsed,
                        interactions_parser.interaction_parser(ROOT))


if __name__ == "__main__":
    unittest.main()
