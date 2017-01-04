"""
This file parses the current world state. It should contain the necessary items
to load a game, that is to say the current map, the entities...
"""

# -*- coding : utf-8 -*-

from collections import OrderedDict
import parsing.parsing_utils as parsing_utils

def parse_world(world_file):
    """ Parses the file describing the world, and returns the adequate
    dictionnaries.
    """

    root = parsing_utils.try_open_and_parse(world_file)
    # Getting the params and the entities on the map.
    params = root.findall('Params')
    entities = root.find('Entities')
    objects = root.find('Objects')
    maps = root.find('Maps')
    ident = root.find("Ident")
    params_dict = OrderedDict()
    entities_list = []
    objects_list = []
    maps_list = []
    if params is not []:
        for _params in params:
            for param in _params.getchildren():
                if param.attrib.get('id'):
                    params_dict.update({param.tag: {"id": param.attrib["id"]}})
                else:
                    params_dict.update({param.tag: param.text})
    if entities is not None:
        for entity in entities.getchildren():
            entities_list += [entity.attrib]
    if objects is not None:
        for object_ in objects.getchildren():
            objects_list += [object_.attrib]
    if maps is not None:
        for map_ in maps.getchildren():
            maps_list += [map_.attrib]
    return OrderedDict({
        'name' : 'world',
        'Entities': entities_list,
        'params': params_dict,
        'Objects': objects_list,
        'Maps': maps_list,
        'ident' : parsing_utils.format_type(ident.text)
    })
