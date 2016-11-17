# -*- coding: utf-8 -*-

"""
This is the main module file.
This file mainlky handle the global loading of the world from the xml files.
The most important methods are the game_parser one and the save_xml.
"""

import xml.etree.ElementTree as ET
import sys
from os.path import isfile
import map_parser
import entity_parser
import world_parser
import parsing_utils
import objects_parser
import actions_parser
import interactions_parser


def check_files(*args):
    """Checks if all given files are present in the directory."""
    assert args
    collect = []
    data = list(args)
    for arg in data:
        if not isfile(arg):
            collect.append(arg)
    if collect:
        print("The following files were not found : %s " % ", ".join(collect))
        return False
    return True

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
        world_file = [world_file.text]
        map_files = world_tag.findall('Map')
        if not map_files:
            parsing_utils._fail_not_found("Map")
        map_files = [ x.text for x in map_files]
        cells = world_tag.findall('Cell')
        if not cells:
            parsing_utils._fail_not_found("Cell")
        cells = [cell.text for cell in cells]
        objects = world_tag.findall('Objects')
        if not objects:
            parsing_utils._fail_not_found("Objects")
        objects = [obj.text for obj in objects]
        objects_types = world_tag.findall('ObjectTypes')
        if not objects_types:
            object_types = []
        objects_types = [obj_t.text for obj_t in objects_types]
    # Gets the available actions files.
    action_tag = root.find('Actions')
    if action_tag is not None:
        action_files = action_tag.findall('Action')
        action_files = [action.text for action in action_files]
    # Gets the available interactions files.
    interactions_tag = root.find('Interactions')
    if interactions_tag is not None:
        interaction_files = interactions_tag.findall('Interaction')
        interaction_files = [interaction.text for interaction in interaction_files]

    # Checks if all files defined in game.xml as game files are present in
    # the directories.
    if not check_files(*world_file, *map_files,
                       *cells, *action_files, *interaction_files,
                       *objects, *objects_types):
        sys.exit(1)

    # Gets the map data.
    map_data = map_parser.collect_map_data(map_files)

    # Gets the world data.
    world_data = world_parser.parse_world(world_file[0])

    available_cells = map_parser.collect_cells_data(cells)
    return {
        "maps": map_data,
        "cells": available_cells,
        "world": world_data,
        "actions": actions_parser.parse_multiple_files(*action_files),
        "interactions": interactions_parser.interactions_files_parser(*interaction_files),
        "objects": objects_parser.multiple_files_object_parser(*objects),
        "objects_types": objects_parser.multiple_files_object_types_parser(*objects_types),
            }
