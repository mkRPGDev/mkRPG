"""
This file handles parsing of interactions files, that is to say files that
describe the reactions of the game to the keyboard, and the events associated to
these interactions.
"""

import xml.etree.ElementTree as ET
import sys
import parsing_utils
import actions_parser

def interaction_parser(interaction_tag):
    """Parses one interaction tag. An interaction tag is very simple, it
    contains a curses keycode, a target and and event.
    """

    interaction = {}

    _key  = interaction_tag.find('key')
    if _key is None:
        parsing_utils._fail_not_found('key')
    if _key.get('val') is None:
        print("Tag %s found, but value not present" % 'key')
    key = parsing_utils.format_type(_key.get('val'))
    for tag in ['target', 'event']:
        val = interaction_tag.find(tag)
        if val is None:
            parsing_utils._fail_not_found(tag)
        if val.get("val") is None:
            print("Tag  %s found, but value not present" % tag)
            sys.exit(1)
        interaction.update({tag : val.get("val")})
    return(key, interaction)

def interactions_parser(interaction_xml):
    """This function parses a whole file, and returns the dictionnary of all
    actions described in the file.
    """

    interactions = parsing_utils.try_open_and_parse(interaction_xml)
    keys_found = []
    interactions_dict = {}

    for interaction in interactions.findall('Interaction'):
        key, _interaction = interaction_parser(interaction)
        if key in keys_found:
            print("Keycode found twice. Something wrong")
        else:
            keys_found.append(key)
            interactions_dict.update({key: _interaction})
    return interactions_dict

def interactions_files_parser(interactions_files):
    """This function parses a list of files, in order to find all interactions
    described in these files. It provides some safety, since it checks that
    every keycode is used at most once.
    """
    data = parsing_utils.parse_multiple_files(interactions_files,
                                              interactions_parser)
    return data

def get_all_actions(interactions):
    """Returns a list with all action names defined in ```interaction```.
    It is useful in order to check that no action called in an Action
    tagged file is never called by an interaction.
    No sanity check is done here, since interactions should be
    well-formed when it arrives here.
    """
    return parsing_utils.collect_data('event', interactions)

def check_actions(interactions, actions):
    """Checks if all actions called by an interaction exist."""

    interaction_names = get_all_actions(interactions)
    action_names = actions_parser.get_all_names(actions)
    return (interaction_names <= action_names)
