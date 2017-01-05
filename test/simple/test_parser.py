# -*- coding: utf-8 -*-

import sys
import xml.etree.ElementTree as ET

sys.path.append('../../src/')
sys.path.append('../../src/parsing/')

import map_parser, world_parser, global_parsing
import objects_parser

def formatter(l):
    """ Formats the output, to check tests """
    if type(l) == dict:
        for (key,item) in l.items():
            print(" * Result: %s -> %s" % (key, item))
    else:
        for item in l:
            print(" * Result: %s" % item)

def test_map_parser():
    print("Testing map parser")
    map_parsed = map_parser.map_parser("maps/map.xml")
    formatter(map_parsed)

def test_cell_parser():
    print("Testing cell parser")
    cells_parsed = map_parser.parse_all_cells("maps/cell.xml")
    formatter(cells_parsed)

def test_cells_multiple():
    print("Testing multiple files cell parser")
    cells_parsed = map_parser.collect_cells_data("maps/cell.xml", "maps/cell.xml")
    formatter(cells_parsed)

def test_world_parser():
    print("Testing world parser")
    parsed = world_parser.parse_world("world.xml")
    formatter(parsed)

def test_collect_data():
    print("Testing data collection")
    cells = map_parser.parse_all_cells("maps/cell.xml")
    worlds = world_parser.parse_world("world.xml")
    entities = global_parsing.collect_data('entities', cells, worlds)
    formatter(entities)

def test_collect_map_data():
    print("Testing to collect different map definitions")
    map_files = "maps/map.xml", "maps/map2.xml"
    collected = map_parser.collect_map_data(map_files)
    formatter(collected)

def test_game_parser():
    print("Testing main game parser")
    data = global_parsing.game_parser("game.xml")
    formatter(data)


def test_check_files():
    print("Testing check_file function, a file presence safety check")
    if global_parsing.check_files(*["interactions.xml", "game.xml"], *["entities/entity.xml"]):
        print("Test 1 : All files presents -> Passed")
    else:
        print("Test 1 : All files presents -> Not Passed !")
    if not global_parsing.check_files("interactions.xml", "game_bis.xml", "entities/entity.xml"):
        print("Test 2 : Files are missing -> Passed")
    else:
        print("Test 2 : Files are missing  -> Not Passed !")

def test_object_parser():
    print("Testing the object parser for one object tag")
    tag = ET.parse("entities/pomme.xml").getroot().find('Object')
    data = objects_parser.object_parser(tag)
    formatter(data)

def test_objects_parser():
    print("Testing the object parser for one file")
    data = objects_parser.objects_parser("entities/pomme.xml")
    formatter(data)


def test_multiple_files_objects_parser():
    print("Testing the object parser for one file")
    data = objects_parser.multiple_files_object_parser("types/pomme.xml", "poire.xml")
    formatter(data)

def test_object_type_parser():
    print("Testing the object type parser for one object type tag")
    tag = ET.parse("../snake/types/corps.xml").getroot().find('ObjectType')
    data = objects_parser.object_type_parser(tag)
    formatter(data)

def test_objects_type_parser():
    print("Testing the object type parser for one file")
    data = objects_parser.objects_type_parser("corps.xml")
    formatter(data)

def test_global_parsing():
    print("Test global parser")
    data = global_parsing.game_parser("game.xml")
    formatter(data)

if __name__=="__main__":
#    test_map_parser()
#    test_cell_parser()
#    test_cells_multiple()
#    test_entity_parser()
#    test_multiple_files_entities()
#    test_world_parser()
#    test_check_entity()
#    test_collect_data()
#    test_collect_map_data()
#    test_game_parser()
#    test_actions_parser()
#    test_multiple_actions_parser()
    test_interaction_parser()
#    test_file_interaction_parser()
#    test_files_interaction_parser()
#    test_get_all_actions()
#    test_check_actions()
#    test_check_files()
#    test_object_parser()
#    test_objects_parser()
#    test_multiple_files_objects_parser()
#    test_object_type_parser()
#    test_objects_type_parser()
#    test_global_parsing()
