# -*- coding: utf-8 -*-

import sys
sys.path.append('../../src/parsing/')

import map_parser, entity_parser

def formatter(l):
    """ Formats the output, to check tests """
    for (key,item) in l.items():
        print(" * Result: %s -> %s" % (key, item))

def test_map_parser():
    map_parsed = map_parser.map_parser("maps/map.xml")
    formatter(map_parsed)


def test_cell_parser():
    cells_parsed = map_parser.parse_all_cells("maps/cell.xml")
    formatter(cells_parsed)

def test_entity_parser():
    enitities_parsed = entity_parser.parse_entities("entities/entity.xml")
    formatter(enitities_parsed)


if __name__=="__main__":
    test_map_parser()
    test_cell_parser()
    test_entity_parser()
