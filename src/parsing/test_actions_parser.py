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
from parsing import actions_parser

ROOT = ET.parse("parsing/test_files/actions.xml").getroot().find("Action")

class GetTagTestCase(unittest.TestCase):
    def setUp(self):
        self.parsed = 'emitter'

    def test_get_tag(self):
        self.assertEqual(self.parsed,
                        actions_parser.get_tag(ROOT.find("Order"),
                                               "target", optionnal=True))

    def test_fail_optionnal_get_tag(self):
        self.assertEqual(None,
                         actions_parser.get_tag(ROOT.find("Order"),
                                               "none", optionnal=True))


class ParseOrderTestCase(unittest.TestCase):
    def setUp(self):
        self.parsed_1 = OrderedDict([('type', 'set'), ('value',
            '(self.x+1)%self.map.width'), ('param', 'x'), ('target',
                'emitter')])

    def test_parse_order(self):
        self.assertEqual(self.parsed_1, actions_parser.parse_order(ROOT.find("Order")))

class ParseAction(unittest.TestCase):
    def setUp(self):
        self.parsed_1 = ('moveright', [OrderedDict([('type', 'set'), ('value',
            '(self.x+1)%self.map.width'), ('param', 'x'), ('target',
                'emitter')])])


    def test_parse_action(self):
        self.assertEqual(self.parsed_1, actions_parser.parse_action(ROOT))

if __name__ == "__main__":
    unittest.main()
