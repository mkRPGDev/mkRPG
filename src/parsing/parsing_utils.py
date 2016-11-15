"""
This module contains many utilities that are called almost everywhere
during the parsing.
It is more a toolbox than anything.
"""
import sys
import xml.etree.ElementTree as ET

def try_open_and_parse(game_xml):
    """
    This function tries to open the file ```game_xml```, and to parse it.
    The program exits if one of both action fails.
    """
    try:
        parsed_file = ET.parse(game_xml)
    except IOError as exception:
        print("Couldn't find or open file %s. Are you in the good directory ?"
              % abspath(game_xml))
        sys.exit(1)
    except ET.ParseError as exception:
        print("The xml format seems to be not well-formed: ligne:%d, column:%d"
              % (exception.position[0], exception.position[1]))
        sys.exit(1)

    root = parsed_file.getroot()
    return root

def _fail_not_found(tag):
    print("Tag %s not found" % tag)
    sys.exit(1)
