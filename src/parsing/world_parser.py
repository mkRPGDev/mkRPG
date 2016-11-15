"""
This file parses the current world state. It should contain the necessary items
to load a game, that is to say the current map, the entities...
"""

# -*- coding : utf-8 -*-

import xml.etree.ElementTree as ET
from os.path import abspath
import sys

import parsing_utils

def parse_world(world_file):
    """ Parses the file describing the world, and returns the adequate
    dictionnaries.
    """

    root = parsing_utils.try_open_and_parse(world_file)
    # Getting the params and the entities on the map.
    params = root.find('Params')
    entities = root.find('Entities')
    if params is not None:
        params_dict = {}
        for param in params.getchildren():
            params_dict.update({param.tag: param.text})
    if entities is not None:
        entities_list = []
        for entity in entities.getchildren():
            entities_list += [entity.text]
    return({'entities': entities_list, 'params': params_dict})
#    for other in root:
#        if other.tag != 'Entities' and other.tag != 'Params':

