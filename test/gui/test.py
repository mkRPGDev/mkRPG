# -*- coding: utf-8 -*-

import sys
sys.path.append("../../src/")
import xml.etree.ElementTree as ET

import parsing.gui_parser as gui_parser

def test_text_field():
    root = ET.parse("textfield.xml").getroot().find("TextField")
    data = gui_parser.parse_element(root)
    print(data)

def test_window():
    data = gui_parser.parse_file("widget.xml")
    print(data)

if __name__ == "__main__":
    test_window()
