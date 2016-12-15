"""
This module handles the parsing of entities.
The entities, if present, are present in the files tagged by Entity in the
world.xml file.
It is possible to present several Entity files, for example for a clearer
separation for the Dungeon Master.

An entity may have a default position, an one or more characteristics.
These characteristics are independent of the game generator, and though are
handled by a dictionary. However, it always have a name. Moreover,
it always have a picture, its grpahical representation in the GUI client
"""

#-*- coding : utf-8 -*-

import parsing_utils


def get_characteristics(_characteristics):
    """
    Iterate over all characteristics.
    Be careful ! This assume that each characteristic is a leaf of the
    tree. Is it legit?
    It returns a dictionnary which keys are characteristic names, and values
    are the characteristics.
    Each characteristic should be an integer.
    """
    characteristics = {}
    for _characteristic in _characteristics.getchildren():
        if _characteristic.attrib.get("id"):
            value = parsing_utils.format_type(_characteristic.attrib.get("id"))
            characteristics.update({_characteristic.tag: {'id':value}})
        else:
            value = parsing_utils.format_type(_characteristic.text)
            characteristics.update({_characteristic.tag: value})
    return characteristics

def parse_entity(entity_element):
    """
    Parses an entity element from the xml tree.
    """
    name = entity_element.attrib['name']
    if name is None:
        # No name, exit now and alert the user.
        parsing_utils.fail_not_found("name")
    _ident = entity_element.find("Ident")
    if _ident is None:
        # No identifier, exit and alert the user.
        parsing_utils.fail_not_found("Ident")
    #The minimum expected for an entitiy
    answer = {
        'name': name,
        'type' : "Entity",
        "ident": parsing_utils.format_type(_ident.text)
    }
    _params = entity_element.find('Params')
    if _params is None:
        parsing_utils.fail_not_found("Params")
    picture = _params.find("picture")
    if picture is None:
        parsing_utils.fail_not_found("picture")
    answer.update({'params': get_characteristics(_params)})
    return answer

def get_names(entities):
    """Get the names of the entities."""
    return entities.keys()

def parse_entities(entity_xml):
    """ Parses an entity file, and returns the entities described in the file.
    """
    root = parsing_utils.try_open_and_parse(entity_xml)
    _entities = root.findall('Entity')
    entities = []
    for entity in _entities:
        entities.append(parse_entity(entity))
    return entities

def parse_multiple_entities(*entities_list):
    """ Parses a list of entities, and return the dictionary of all data,
    without redundancy.
    """
    return parsing_utils.parse_multiple_files(parse_entities, *entities_list)

def check_entity(entities_found, entities_listed):
    """Checks if all entities found in files world.xml, and cell.xml and others
    are present in entites tagged files.
    """
    names = get_names(entities_listed)
    try:
        for entity in entities_found:
            if entity not in names:
                raise KeyError(entity)
        return True
    except KeyError as entity:
        print("Entity %s not found in defined entities" % entity)
        return False
