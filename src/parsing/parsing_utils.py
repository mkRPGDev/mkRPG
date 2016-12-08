"""
This module contains many utilities that are called almost everywhere
during the parsing.
It is more a toolbox than anything.
"""

# -*- coding : utf-8 -*-

import sys
import xml.etree.ElementTree as ET
import re
from os.path import abspath

INT = re.compile('[0-9]+$')
FLOAT = re.compile(r'[0-9]+[\.,][0-9]$')

def try_open_and_parse(xml_file):
    """
    This function tries to open the file ```xml_file```, and to parse it.
    The program exits if one of both action fails.
    """
    try:
        parsed_file = ET.parse(xml_file)
    except IOError as exception:
        print("Couldn't find or open file %s. Are you in the good directory ?"
              % abspath(xml_file))
        sys.exit(1)
    except ET.ParseError as exception:
        print("The xml format seems to be not well-formed: ligne:%d, column:%d"
              % (exception.position[0], exception.position[1]))
        sys.exit(1)

    root = parsed_file.getroot()
    return root

def fail_not_found(tag):
    """ Error when a tag is not found in a file, and should be in."""
    print("Tag %s not found" % tag)
    sys.exit(1)

def format_type(string):
    """ If the string can be converted to an int or a float, converts it."""

    if re.match(INT, string):
        return int(string)
    elif re.match(FLOAT, string) or string == 'inf':
        return float(string)
    else:
        return string

def collect_data(key, *args):
    """ Collects all items corresponding to key argument in *args.  It realizes
    a depth-first travel of the dictionary, calling itself recursively on the
    structures."""
    collection = set()
    if args:
        for arg in args:
            if key in arg:
                collection.add(arg[key])
            elif isinstance(arg, dict):
                for sub_key in arg:
                    collection |= collect_data(key, arg[sub_key])
    return collection


def parse_multiple_files(parsing_method, *args):
    """Parses the given file lists, and returns the dictionary containing the
    whole data parsed by the parsing_method function.
    """
    all_data = []
    for data_file in args:
        new_data = parsing_method(data_file)
        for key in new_data:
            all_data.append(key)
    return all_data
