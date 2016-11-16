"""
This file parses the current world state. It should contain the necessary items
to load a game, that is to say the current map, the entities...
"""

# -*- coding : utf-8 -*-

import xml.etree.ElementTree as ET
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
    objects = root.find('Objects')
    params_dict = {}
    entities_list = []
    objects_list = []
    if params is not None:
        for param in params.getchildren():
            params_dict.update({param.tag: param.text})
    if entities is not None:
        for entity in entities.getchildren():
            entities_list += [entity.text]
    if objects is not None:
        for object_ in objects.getchildren():
            objects_list += [(object_.attrib.get('id'), object_.text)]
    return {'entities': entities_list, 'params': params_dict, 'objects': objects_list}

