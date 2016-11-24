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
    answer = {'name': name}
    cell_params = cell_object.find('Params')
    for param in cell_params.getchildren():
        answer.update({param.tag: param.text})
    _entities = cell_object.find("Entities")
    if _entities is not None:
        entities = _entities.findall("Entity")
        answer.update({'entities': [entity.attrib for entity in entities]})
    return answer

def parse_all_cells(cell_xml):
    """ Parses all cells defined in the given file."""
    root = ET.parse(cell_xml)
    cells = root.findall("CellType")
    list_cells = []
    for cell in cells:
        list_cells.append(parse_cell(cell))
    return list_cells

def collect_cells_data(*cell_files):
    """ Collects all cells descriptions in the given files."""
    return parsing_utils.parse_multiple_files(parse_all_cells, *cell_files)

def map_parser(map_xml):
    """
    The main parser for the map xml file.
    """
    root = parsing_utils.try_open_and_parse(map_xml)
    name = root.attrib['name']
    answer = {'name': name}
    params = root.find("Params")
    answer.update({'params' : {}})
    for _param in params.getchildren():
        if _param.attrib.get("id"):
            value = {"id" : parsing_utils.format_type(_param.attrib.get("id"))}
        else:
            value = parsing_utils.format_type(_param.text)
            answer['params'].update({_param.tag : value})

    available_cells = []
    cells = root.find('Cells')
    for cell in cells.findall("Cell"):
        available_cells.append(cell.attrib)
    celltype = root.find("CellType")
    cell = parse_cell(celltype)
    available_cells.append(cell)
    answer.update({"Cell":available_cells})
    # Getting the default cell.
    return answer


def collect_map_data(map_files):
    """ Collects all map descriptions in the given files."""
    map_data = []
    for map_file in map_files:
        data = map_parser(map_file)
        map_data.append(data)
    return map_data

