"""
This is the parser for the actions files.
Recall that the actions should all be defined in such a file.
"""
# -*- coding : utf-8 -*-

import xml.etree.ElementTree as ET
import sys
import parsing_utils

def get_tag(order_tag, tag, optionnal=False):
    """Gets the value of an argument. If optionnal is set to True, it returns
    None if nothing is found, it will stop the game else.
    """
    _tag = order_tag.find(tag)
    if _tag is None:
        if not optionnal:
            parsing_utils._fail_not_found(tag)
        else:
            return None
    if _tag.attrib.get("id"):
        return _tag.attrib
    if _tag.attrib.get("val") is None:
        print("Tag %s found, but val not found." % tag)
        sys.exit(1)
    return _tag.attrib.get("val")

def parse_order(order_tag):
    """ Parses an order tag. Returns the type of order, and the code to run."""
    res = {}
    type_order = order_tag.attrib.get("type")
    if type_order is None:
        parsing_utils._fail_not_found("Type in order tag")
    res.update({"type": type_order})

    for tag in ["value", "event", "param", "target", "init", "once", "base"]:
        value = get_tag(order_tag, tag, optionnal=True)
        if value:
            res.update({tag: value})
    return res


def parse_action(action_tag):
    """ Parses an action tag. It return the dictionnary with the interesting
    values.
    """
    # What event can produce this action ?
    _event = action_tag.find('Event')
    if _event is None:
        parsing_utils._fail_not_found('Event')
    event = _event.attrib.get("val")

    # What orders are defined for this action ?
    _orders = action_tag.findall('Order')
    if not _orders:
        parsing_utils._fail_not_found('Orders')
    orders = []
    for order in _orders:
        orders += [ parse_order(order) ]
    return(event, orders)

def parse_actions(action_xml):
    """Parses a whole action file.
    It should return a dictionnay {event: orders} of all described actions.
    Please note that only actions are being looked for, other tag will simply
    be ignored by this function. This shouldn't bother anybody however, since
    other tags would break the semantics of this file.
    """
    actions = parsing_utils.try_open_and_parse(action_xml)

    actions_list = []
    # Get all actions, and save it in a dictionnary
    for _action in actions.findall("Action"):
        event, orders = parse_action(_action)
        actions_list.append({'event': event, 'orders': orders})
    return actions_list

def parse_multiple_files(*actions_files):
    """Parses multiple files. Broadly speaking, it parses sequentially all
    files, and concatenates all answers.
    """
    return parsing_utils.parse_multiple_files(parse_actions, *actions_files)


def get_all_names(actions):
    """ Returns all action names present in the given actions dictionnary."""
    assert type(actions)==dict
    names = set(actions.keys())
    return names
