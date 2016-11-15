"""
This module handles xml parsing for maps description files.
There should be one map description per xml file.
"""

# -*- coding : utf-8 -*-

import xml.etree.ElementTree as ET
import parsing_utils

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
    """ Parses all cells defined in the given file."""
    root = ET.parse(cell_xml)
    cells = root.findall("CellType")
    dict_cells = {}
    for cell in cells:
        dict_cells.update(parse_cell(cell))
    return dict_cells

def collect_cells_data(cell_files):
    """ Collects all cells descriptions in the given files."""
    cells_data = {}
    for cell_file in cell_files:
        data = parse_all_cells(cell_file)
        cells_data.update(data)
    return cells_data

def map_parser(map_xml):
    """
    The main parser for the map xml file.
    """
    root = parsing_utils.try_open_and_parse(map_xml)
    name = root.attrib['name']
    answer = {}
    map_size = get_size(root.find('Params'))
    answer.update({'size': map_size})
    available_cells = {}
    for cell in root.findall('CellType'):
        available_cells = {**available_cells, **parse_cell(cell)}
    answer.update({'default cells': available_cells})
    return (name, answer)


def get_size(tree):
    """
    Gets the size of the map.
    """
    height = int(tree.find('height').text)
    width = int(tree.find('width').text)
    return (height, width)


def collect_map_data(map_files):
    """ Collects all map descriptions in the given files."""
    map_data = {}
    for map_file in map_files:
        name, data = map_parser(map_file)
        map_data.update({name : data})
    return map_data

