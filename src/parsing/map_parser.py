"""
This module handles xml parsing for maps description files.
"""

# -*- coding : utf-8 -*-

import xml.etree.ElementTree as ET


def parse_cell(cell_object):
    """
    Parses a CellType attribute.
    """
    name = cell_object.attrib['name']
    cell_params = cell_object.find('Params')
    altitude = int(cell_params.find('altitude').text)
    picture = cell_params.find('picture').text
    return {name : (altitude, picture)}

def map_parser(map_xml):
    """
    The main parser for the map xml file.
    """
    root = ET.parse(map_xml)
    map_characteristics = get_size(root.find('Params'))
    print("Map characteristics: " + str(map_characteristics))
    available_cells = {}
    for cell in root.findall('CellType'):
        available_cells = { **available_cells, **parse_cell(cell) }
    print(available_cells)


def get_size(tree):
    """
    Gets the size of the map.
    """
    height = int(tree.find('height').text)
    width = int(tree.find('width').text)
    return (height, width)

def gen_map(cells_specs):
    print("Unimplemented")
