"""
This is the parser for the actions files.
Recall that the actions should all be defined in such a file.
"""
# -*- coding : utf-8 -*-

import xml.etree.ElementTree as ET
import sys
import parsing_utils

def parse_order(order_tag):
    """ Parses an order tag. Returns the type of order, and the code to run."""
    res = {}
    type_order = order_tag.attrib.get("type")
    if type_order is None:
        parsing_utils._fail_not_found("Type in order tag")
    res.update({"type": type_order})

    # Gets the target of the order.
    _target = order_tag.find("target")
    # Gets the params affected by the order.
    _param = order_tag.find('param')
    # Gets the code to run in in order to execute the order.
    _value = order_tag.find('value')

    if _target is None:
        parsing_utils._fail_not_found("target")
    if _param is None:
        parsing_utils._fail_not_found("param")
    if _value is None:
        parsing_utils._fail_not_found("value")
    # Gets the effective values of the former fields.
    res.update({
        "target": _target.attrib.get("val"),
        "param": _param.attrib.get("val"),
        "value": _value.attrib.get("val"),
        })
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

    actions_dict = {}
    # Get all actions, and save it in a dictionnary
    for _action in actions.findall("Action"):
        event, orders = parse_action(_action)
        actions_dict.update({event:orders})
    return actions_dict

def parse_multiple_files(actions_files):
    """Parses multiple files. Broadly speaking, it parses sequentially all
    files, and concatenates all answers.
    """

    assert actions_files
    all_actions = {}
    for actions_file in actions_files:
        new_actions = parse_actions(actions_file)
        for key in new_actions.keys():
            if all_actions.get(key) is not None:
                print("Action %s was found in multiple files" % key)
            else:
                all_actions.update({key: new_actions[key]})
    return all_actions


def get_all_names(actions):
    """ Returns all action names present in the given actions dictionnary."""
    assert type(actions)==dict
    names = set(actions.keys())
    return names
