"""
This file handles the parsing of xml files in order to find all paths.
In other xml files, images are identified by a number, which is bound
in xml files.
"""

import sys
from os.path import sep
import xml.etree.ElementTree as ET
import parsing_utils

def parse_image(image_tag):
    _identifier = image_tag.find("Identifier")
    _path = image_tag.find("Path")

    if _identifier is None:
        parsing_utils._fail_not_found("Identifier")
    identifier = parsing_utils.format_type(_identifier.text)
    if type(identifier) != int:
        print("Fail : identifier couldn't be cast in integer.")
        sys.exit(1)
    if _path is None:
        parsing_utils._fail_not_found("Path")
    path = _path.text

    return {identifier:path.replace("/", sep)}

def parse_file_image(image_file):
    images_dict = {}
    root = parsing_utils.try_open_and_parse(image_file)

    images = root.findall("Image")
    for image in images:
        images_dict.update(parse_image(image))
    return(images_dict)
