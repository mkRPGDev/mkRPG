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
    answer = {name : {}}
    cell_params = cell_object.find('Params')
    _altitude = cell_params.find('altitude')
    if _altitude:
        altitude = int(_altitude.text)
        answer[name].update({'altitude' : altitude})
    picture = cell_params.find('picture').text
    answer[name].update({'picture': picture})
    _entities = cell_object.find("Entities")
    if _entities:
        entities = _entities.findall("Entity")
        answer[name].update({'entities': [entity.text for entity in entities]})
    return answer

def parse_all_cells(cell_xml):
    root = ET.parse(cell_xml)
    cells = root.findall("CellType")
    dict_cells = {}
    for cell in cells:
        dict_cells.update(parse_cell(cell))
    return dict_cells

def map_parser(map_xml):
    """
    The main parser for the map xml file.
    """
    root = ET.parse(map_xml)
    answer = {}
    map_size = get_size(root.find('Params'))
    answer.update({'size': map_size})
    available_cells = {}
    for cell in root.findall('CellType'):
        available_cells = { **available_cells, **parse_cell(cell) }
    answer.update({'default cells': available_cells})
    return answer


def get_size(tree):
    """
    Gets the size of the map.
    """
    height = int(tree.find('height').text)
    width = int(tree.find('width').text)
    return (height, width)

def gen_map(cells_specs):
    print("Unimplemented")
