"""
This is the main module file.
This file mainlky handle the global loading of the world from the xml files.
The most important methods are the game_parser one and the save_xml.
"""

# -*- coding: utf-8 -*-

import sys
from os.path import isfile, abspath, dirname, sep, join
import parsing.map_parser as mp
import parsing.entity_parser as ep
import parsing.world_parser as wp
import parsing.parsing_utils as parsing_utils
import parsing.objects_parser as op
import parsing.actions_parser as ap
import parsing.interactions_parser as ip


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

    dir_path = dirname(abspath(game_xml))

    # Try to open and parse the given file.
    root = parsing_utils.try_open_and_parse(game_xml)

    # The result dictionnary
    result = {}

    # Gets the important data.
    world_tag = root.find('World')
    if world_tag is not None:
        # Parsing the world tag. We here get the paths of the xml files
        # that handle the map, the cells, the entities...
        world_file = world_tag.find("World")
        if world_file is None:
            parsing_utils.fail_not_found("World")
        world_file = [join(dir_path, world_file.text.replace('/', sep))]

        # Gets the world data.
        world_data = wp.parse_world(world_file[0])
        if world_data:
            result.update({"World": [world_data]})

        map_files = world_tag.findall('Map')
        if not map_files:
            parsing_utils.fail_not_found("Map")
        map_files = [join(dir_path, x.text.replace('/', sep))
                     for x in map_files]

        # Gets the map data.
        default_cells, map_data = mp.collect_map_data(map_files)

        result.update({"Map" : map_data})

        cells = world_tag.findall('Cell')
#        if not cells:
#            parsing_utils.fail_not_found("Cell")
        cells = [join(dir_path, cell.text.replace('/', sep))
                 for cell in cells]

        available_cells = mp.collect_cells_data(*cells)

        if cells or default_cells:
            result.update({"Cell": available_cells + default_cells})

        entities_files = world_tag.findall('Entities')
        if not entities_files:
            parsing_utils.fail_not_found("Entities")
        entities_files = [join(dir_path, x.text.replace('/', sep))
                          for x in entities_files]

        result.update({"Entity" : ep.parse_multiple_entities(
            *entities_files)})


        objects = world_tag.findall('Objects')
        objects = [join(dir_path, obj.text.replace('/', sep)) for obj in objects]
        if objects:
            result.update({"Object": op.multiple_files_object_parser(*objects)})

        objects_types = world_tag.findall('ObjectTypes')
        if not objects_types:
            objects_types = []
        objects_types = [join(dir_path, obj_t.text.replace('/', sep)) for obj_t
                         in objects_types]
        if objects_types:
            result.update({"ObjectType": op.multiple_files_otypes_parser(
                *objects_types)})
    # Gets the available actions files.
    action_tag = root.find('Actions')
    if action_tag is not None:
        action_files = action_tag.findall('Action')
        action_files = [join(dir_path, action.text.replace('/', sep)) for
                        action in action_files]

    result.update({"Actions": ap.parse_multiple_files(*action_files)})
    # Gets the available interactions files.
    interactions_tag = root.find('Interactions')
    if interactions_tag is not None:
        interaction_files = interactions_tag.findall('Interaction')
        interaction_files = [join(dir_path, interaction.text.replace('/', sep))
                             for interaction in interaction_files]

    result.update({"Interactions": ip.interactions_files_parser(
        *interaction_files)})
    # Checks if all files defined in game.xml as game files are present in
    # the directories.
    if not check_files(*world_file, *map_files,
                       *cells, *action_files, *interaction_files,
                       *objects, *objects_types, *entities_files):
        sys.exit(1)


    return result
