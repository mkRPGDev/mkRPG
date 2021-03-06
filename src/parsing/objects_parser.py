"""
This file handles the parsing of the objects defined in the game.
"""

# -*- coding : utf-8 -*-

from collections import OrderedDict
import parsing.parsing_utils as parsing_utils

def object_parser(object_tag):
    """ Parses an object tag, and returns the dictionnary
    containing all the important data.
    """

    # Gets the name of the object, and stops if it is not found.
    name = object_tag.attrib.get("name")
    if name is None:
        parsing_utils.fail_not_found("name")

    # Gets the identifier of the object
    _ident = object_tag.find("Ident")
    if _ident is None:
        parsing_utils.fail_not_found("Ident")

    # Gets the params of the object
    _params = object_tag.find("Params")
    if _params is None:
        parsing_utils.fail_not_found("Params")

    _picture = _params.find("picture")
    if _picture is None:
        parsing_utils.fail_not_found("picture")

    object_params = OrderedDict({
        'name': name,
        'type' : 'Object',
        'ident' : parsing_utils.format_type(_ident.text),
        'params': OrderedDict()
    })
    for tag in _params.getchildren():
        object_params['params'].update({tag.tag: tag.text})
    return object_params

def object_type_parser(object_type_tag):
    """Parses an object type. An object type can be used as a factory to create
    new objects that are instanciated at runtime.
    """

    name = object_type_tag.attrib.get("name")
    if name is None:
        parsing_utils.fail_not_found("name")

    # Gets the identifier of the object
    _ident = object_type_tag.find("Ident")
    if _ident is None:
        parsing_utils.fail_not_found("Ident")

    _params = object_type_tag.find("Params")

    if _params is None:
        parsing_utils.fail_not_found("Params")
    params_dict = OrderedDict({'name': name})
    params_dict.update({'ident': parsing_utils.format_type(_ident.text)})
    params_dict.update({'params' : OrderedDict()})
    for param in _params:
        if param.attrib.get('id'):
            params_dict['params'].update({param.tag: {"id": param.attrib.get("id")}})
        else:
            params_dict['params'].update({param.tag: param.text})
    return params_dict


def objects_parser(object_file):
    """
    Parses a file defining some objects (In game, not Python objects...).
    It returns the dictionary with all objects defined in the file.
    """

    root = parsing_utils.try_open_and_parse(object_file)

    objects_list = []
    for object_tag in root.findall("Object"):
        object_params = object_parser(object_tag)
        objects_list.append(object_params)
    return objects_list

def objects_type_parser(object_file):
    """
    Parses a file defining some objects (In game, not Python objects...).
    It returns the dictionary with all objects defined in the file.
    """

    root = parsing_utils.try_open_and_parse(object_file)

    objects_list = []
    for object_tag in root.findall("ObjectType"):
        object_params = object_type_parser(object_tag)
        objects_list.append(object_params)
    return objects_list

def multiple_files_object_parser(*files_list):
    """Parses a list of files, and returns the dictionary containing all data
    defined in these files. Note that if an object is described multiple times,
    the last definition will be kept. Maybe in the future, it will just stop
    the game, in order to fix the issue.
    """
    return parsing_utils.parse_multiple_files(objects_parser, *files_list)

def multiple_files_otypes_parser(*files_list):
    """Parses a list of files, and returns the dictionary containing all data
    defined in these files.
    """
    return parsing_utils.parse_multiple_files(objects_type_parser, *files_list)
