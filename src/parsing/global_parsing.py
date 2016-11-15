# -*- coding: utf-8 -*-

"""
This is the main module file.
This file mainlky handle the global loading of the world from the xml files.
The most important methods are the game_parser one and the save_xml.
"""

import xml.etree.ElementTree as ET
from os.path import abspath
import sys
import map_parser
import entity_parser
import world_parser
import parsing_utils

def check_entity(entities_found, entities_listed):
    """Checks if all entities found in files world.xml, and cell.xml and others
    are present in entites tagged files.
    """
    names = entity_parser.get_names(entities_listed)
    try:
        for entity in entities_found:
            if entity not in names:
                raise KeyError(entity)
        return True
    except KeyError as entity:
        print("Entity %s not found in defined entities" % entity)
        return False

def collect_data(key, *args):
    """ Collects all items corresponding to key argument in *args.  It realizes
    a depth-first travel of the dictionary, calling itself recursively on the
    structures."""
    collection = set()
    if args:
        for arg in args:
            if key in arg:
                collection |= set(arg[key])
            elif isinstance(arg, dict):
                for sub_key in arg:
                    collection |= collect_data(key, arg[sub_key])
    return collection



def game_parser(game_xml):
    """
    This is the main parser. It should parse all xml files from a game, and
    generate a World instance.
    """

    # Try to open and parse the given file.
    root = parsing_utils.try_open_and_parse(game_xml)

    # Gets the important data.
    world_tag = root.find('World')
    if world_tag is not None:
        # Parsing the world tag. We here get the paths of the xml files
        # that handle the map, the cells, the entities...
        world_file = world_tag.find("World")
        if world_file is None:
            parsing_utils._fail_not_found("World")
        map_files = world_tag.findall('Map')
        if not map_files:
            parsing_utils._fail_not_found("Map")
        cells = world_tag.findall('Cell')
        if not cells:
            parsing_utils._fail_not_found("Cell")
    # Gets the available actions files.
    action_tag = root.find('Actions')
    if action_tag is not None:
        action_files = action_tag.findall('file')
    # Gets the available interactions files.
    interactions_tag = root.find('Interactions')
    if interactions_tag is not None:
        interaction_files = interactions_tag.findall('file')

    map_data = map_parser.collect_map_data(
        [map_file.text for map_file in map_files]
    )
    world_data = world_parser.parse_world(world_file.text)
    available_cells = map_parser.collect_cells_data(
        [cell_file.text for cell_file in cells]
    )
    return(map_data, world_data, available_cells)