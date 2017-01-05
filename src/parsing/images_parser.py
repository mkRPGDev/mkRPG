"""
This file handles the parsing of xml files in order to find all paths.
In other xml files, images are identified by a number, which is bound
in xml files.
"""

# -*- coding: utf-8 -*-

import sys
from collections import OrderedDict
from collections import namedtuple
from os.path import sep
import parsing.parsing_utils as parsing_utils

Image = namedtuple("Image", "ascii image")

def parse_image(image_tag, dir_path):
    """ Parses an image tag."""
    _glyph = image_tag.find("Glyph")
    _identifier = image_tag.find("Ident")
    _path = image_tag.find("Path")

    if _glyph is None:
        glyph = ord('?')
    else:
        glyph = parsing_utils.format_type(_glyph.text)
        if not isinstance(glyph, int) or glyph not in range(256):
            print("Fail : glyph couldn't be cast in ascii character.")
            sys.exit(1)

    if _identifier is None:
        parsing_utils.fail_not_found("Ident")
    identifier = parsing_utils.format_type(_identifier.text)
    if not isinstance(identifier, int):
        print("Fail : identifier couldn't be cast in integer.")
        sys.exit(1)
    if _path is None:
        parsing_utils.fail_not_found("Path")
    path = _path.text

    return OrderedDict({identifier:Image(glyph, dir_path+path.replace("/", sep))})

def parse_file_image(image_file, dir_path):
    """ Parses an images file."""
    images_dict = OrderedDict()
    root = parsing_utils.try_open_and_parse(image_file)

    images = root.findall("Image")
    for image in images:
        images_dict.update(parse_image(image, dir_path))
    return images_dict

def parse_multiple_files(dir_path, *images_files):
    """Parses multiple files. Broadly speaking, it parses sequentially all
    files, and concatenates all answers.
    The dir path is used to allow acces to the images back in the client context.
    """
    parsed_data = {}
    for image_file in images_files:
        parsed_data.update(parse_file_image(image_file, dir_path))
    return parsed_data
