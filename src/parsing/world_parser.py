"""
This file parses the current world state. It should contain the necessary items
to load a game, that is to say the current map, the entities...
"""

# -*- coding : utf-8 -*-

import xml.etree.ElementTree as ET
from os.path import abspath
import sys

def parse_world(world_file):
    """ Parses the file describing the world, and returns the adequate
    dictionnaries.
    """

    try :
        root = ET.parse(world_file)
    except IOError as exception:
        print("Couldn't find or open file %s. Are you in the good directory ?"
              % abspath(world_file))
        sys.exit(1)
    except ET.ParseError as except_parse:
        print("Couldn't parse file %s. Are you sure it is true xml?"
              % abspath(world_file))
        sys.exit(1)

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

