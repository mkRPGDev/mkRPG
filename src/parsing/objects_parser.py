"""
This file handles the parsing of the objects defined in the game.
"""

import xml.etree.ElementTree as ET
import parsing_utils

def object_parser(object_tag):
    """ Parses an object tag, and returns the dictionnary
    containing all the important data.
    """

    # Gets the name of the object, and stops if it is not found.
    name = object_tag.attrib.get("name")
    if name is None:
        parsing_utils._fail_not_found("name")

    # Gets the params of the object
    _params = object_tag.find("Params")
    if _params is None:
        parsing_utils._fail_not_found("Params")

    _picture = _params.find("picture")
    if _picture is None:
        parsing_utils._fail_not_found("picture")
    picture = _picture.text

    object_params = {}
    for tag in _params.getchildren():
        object_params.update({tag.tag: tag.text})

    return name, object_params

def object_type_parser(object_type_tag):
    """Parses an object type. An object type can be used as a factory to create
    new objects that are instanciated at runtime.
    """

    name = object_type_tag.attrib.get("name")
    if name is None:
        parsing_utils._fail_not_found("name")

    _params = object_type_tag.find("Params")

    if _params is None:
        parsing_utils._fail_not_found("Params")
    params_dict = {}
    for param in _params:
        if param.attrib.get('id'):
            params_dict.update({param.tag: param.attrib})
        else:
            params_dict.update({param.tag: param.text})
    return (name, params_dict)


def objects_parser(object_file):
    """
    Parses a file defining some objects (In game, not Python objects...).
    It returns the dictionary with all objects defined in the file.
    """

    root = parsing_utils.try_open_and_parse(object_file)

    objects_dict = {}
    for object_tag in root.findall("Object"):
        name, object_params = object_parser(object_tag)
        objects_dict.update({name:object_params})
    return objects_dict

def objects_type_parser(object_file):
    """
    Parses a file defining some objects (In game, not Python objects...).
    It returns the dictionary with all objects defined in the file.
    """

    root = parsing_utils.try_open_and_parse(object_file)

    objects_dict = {}
    for object_tag in root.findall("ObjectType"):
        name, object_params = object_type_parser(object_tag)
        objects_dict.update({name:object_params})
    return objects_dict

def multiple_files_object_parser(*files_list):
    """Parses a list of files, and returns the dictionary containing all data
    defined in these files. Note that if an object is described multiple times,
    the last definition will be kept. Maybe in the future, it will just stop
    the game, in order to fix the issue.
    """
    return parsing_utils.parse_multiple_files(objects_parser, *files_list)

def multiple_files_object_types_parser(*files_list):
    """Parses a list of files, and returns the dictionary containing all data
    defined in these files.
    """
    return parsing_utils.parse_multiple_files(objects_type_parser, *files_list)