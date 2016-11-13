# -*- coding: utf-8 -*-

import sys
import itertools
sys.path.append('../../src/parsing/')

import map_parser, entity_parser, world_parser, global_parsing

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

def test_entity_parser():
    print("Testing entity parser")
    enitities_parsed = entity_parser.parse_entities("entities/entity.xml")
    formatter(enitities_parsed)

def test_world_parser():
    print("Testing world parser")
    parsed = world_parser.parse_world("world.xml")
    formatter(parsed)

def test_check_entity():
    print("Testing entity checker")
    entities = entity_parser.parse_entities("entities/entity.xml")
    cells = map_parser.parse_all_cells("maps/cell.xml")
    entities_found = []
    for cell in cells:
        entities_found += cells[cell]['entities']
    print("Should be True: %s" % global_parsing.check_entity(entities_found, entities))
    print("Should be False: %s" % global_parsing.check_entity(['toto'], entities))

def test_collect_data():
    print("Testing data collection")
    cells = map_parser.parse_all_cells("maps/cell.xml")
    worlds = world_parser.parse_world("world.xml")
    entities = global_parsing.collect_data('entities', cells, worlds)
    formatter(entities)

def test_collect_map_data():
    print("Testing to collect different map definitions")
    map_files = ["maps/map.xml", "maps/map2.xml"]
    collected = map_parser.collect_map_data(map_files)
    formatter(collected)

def test_game_parser():
    print("Testing main game parser")
    data = global_parsing.game_parser("game.xml")
    formatter(data)

if __name__=="__main__":
    test_map_parser()
    test_cell_parser()
    test_entity_parser()
    test_world_parser()
    test_check_entity()
    test_collect_data()
    test_collect_map_data()
    test_game_parser()
