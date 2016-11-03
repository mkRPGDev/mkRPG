# -*- coding: utf-8 -*-

import sys

sys.path.append('../parsing/')
import map_parser

def test_map_parser():
    map_parser.map_parser("../../Test/maps/map.xml")


if __name__=="__main__":
    test_map_parser()
