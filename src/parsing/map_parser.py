"""
This module handles xml parsing for maps description files.
There should be one map description per xml file.
"""

# -*- coding : utf-8 -*-

import parsing_utils

def parse_cell(cell_object):
    """
    Parses a CellType attribute.
    """
    answer = {'type' : 'Cell'}
    name = cell_object.attrib.get('name')
    if name:
        answer.update({'name': name})
    cell_params = cell_object.find('Params')
    answer.update({'params' : {}})
    for param in cell_params.getchildren():
        answer['params'].update({param.tag: param.text})
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
        if cell.attrib.get("id"):
            available_cells.append(cell.attrib)
        else:
            available_cells.append(parse_cell(cell))
    # Gets the default cell value. Returns it separately since it is a cell
    # defined in a map and not in a cell file.
    celltype = root.find("CellType")
    cell = parse_cell(celltype)
    available_cells.append({'id': cell['name']})
    answer.update({"Cell":available_cells})
    # Getting the default cell.
    cell.update({'type' : 'CellType'})
    return cell, answer


def collect_map_data(map_files):
    """ Collects all map descriptions in the given files."""
    map_data = []
    default_cells = []
    for map_file in map_files:
        cell, data = map_parser(map_file)
        map_data.append(data)
        default_cells.append(cell)
    return default_cells, map_data

