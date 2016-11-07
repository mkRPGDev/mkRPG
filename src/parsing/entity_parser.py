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
import xml.etree.ElementTree as ET
import re
import sys

INT = re.compile('[0-9]+$')
FLOAT = re.compile('[0-9]+[\.,][0-9]$')

def _format_type(string):
    if re.match(INT, string):
        return int(string)
    elif re.match(FLOAT, string) or string == 'inf':
        return float(string)
    else:
        return string

def _fail_not_found(tag):
    print("Tag %s not found" % tag)
    sys.exit(1)

def parse_entity(entity_element):
    """
    Parses an entity element from the xml tree.
    """
    name = entity_element.attrib['name']
    if name is None:
        # No name, exit now and alert the user.
        _fail_not_found("name")
    answer = {name : []}
    _position = entity_element.find('Position')
    if _position is not None:
        x, y = _position.find('x'), _position.find('y')
        if x is None or y is None:
            _fail_not_found("x or y")
        answer[name] += [(int(x.text), int(y.text))]
    _params = entity_element.find('Params')
    if _params is None:
        _fail_not_found("Params")
    picture = _params.find("Picture")
    if picture is None:
        _fail_not_found("Picture")
    answer.update({'picture': picture.text})
    _characteristics = entity_element.find('Characteristics')
    if _characteristics is not None:
        characteristics = {}
        # Iterate over all characteristics.
        # Be careful ! This assume that each characteristic is a leaf of the
        # tree. Is it legit ?
        for _characteristic in _characteristics.getchildren():
            characteristics.update({
                _characteristic.tag: _format_type(_characteristic.text)
                })
        answer.update({'characteristics': characteristics})
    return(answer)


def parse_entities(entity_xml):
    parsed = ET.parse(entity_xml)
    if parsed is None:
        print("Couldn't load or parse file")
        sys.exit(1)
    root = parsed.getroot()
    _entities = root.findall('Entity')
    entities = {}
    for entity in _entities:
        entities.update({entity.attrib['name']: parse_entity(entity)})
    return(entities)
