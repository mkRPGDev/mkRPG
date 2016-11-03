"""
This is the main module file.
This file mainlky handle the global loading of the world from the xml files.
The most important methods are the game_parser one and the save_xml.
"""
from . import *
import xml.etree.ElementTree as ET
from os.path import abspath
from os import sep
import sys


def try_open_and_parse(game_xml):
    """
    This function tries to open the file ```game_xml```, and to parse it.
    The program exits if one of both action fails.
    """
    try:
        parsed_file = ET.parse(game_xml)
    except IOError as e:
        print("Couldn't find or open file %s. Are you in the good directory ?"
                % abspath(game_xml))
        sys.exit(1)

    try:
        root = parsed_file.getroot()
    except ET.ParseError as e:
        print("The xml format seems to be not well-formed: ligne:%d, column:%d"
                % (e.position[0], e.position[1]))
        sys.exit(1)
    return root

def game_parser(game_xml):
    """
    This is the main parser. It should parse all xml files from a game, and
    generate a World instance.
    """

    # Try to open and parse the given file.
    root =  try_open_and_parse(game_xml)

    # Gets the important data.
    world_tag= root.find('World')
    if world_tag is not None:
        # Parsing the world tag. We here get the paths of the xml files
        # that handle the map, the cells, the entities...
        world_file = world_tag.find('world')
        if world_file is not None:
            world_file = world_file.text.replace("/", sep)
        map_file = world_tag.find('map')
        if map_file is not None:
            map_file = map_file.text.replace("/", sep)
        cell_file = world_tag.find('cells')
        if cell_file is not None:
            cell_file = cell_file.text.replace("/", sep)
        entities_file = world_tag.find('entities')
        if entities_file is not None:
            entities_file = entities_file.text.replace("/", sep)
        # Gets the available actions.
        actions_file = root.find('World').find('Actions')
        interactions_file = root.find('World').find('Interactions')




