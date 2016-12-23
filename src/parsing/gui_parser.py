"""
This class parses a xml in order to get the description of the user interface
of the game.
"""

# -*- coding: utf-8 -*-

import parsing.parsing_utils as parsing_utils

def get_size(size_tag):
    """ Gets the size of the widget. """
    width = size_tag.find("width")
    height = size_tag.find("height")

    if width is not None and height is not None:
        width_int = parsing_utils.format_type(width.text)
        height_int = parsing_utils.format_type(height.text)

        if isinstance(height_int, int) and isinstance(width_int, int):
            return {"size" : (width_int, height_int)}

    print("Parsing of the size failed")
    exit(1)

def parse_color(color_tag):
    """ Parses any color description, which is always defined the same way,
    with R, G, B and alpha tags.
    """
    _r_tag = color_tag.find("R")
    _g_tag = color_tag.find("G")
    _b_tag = color_tag.find("B")
    _alpha_tag = color_tag.find("alpha")

    if _r_tag is not None and _g_tag  is not None \
        and _b_tag is not None and _alpha_tag is not None:
        r_int = parsing_utils.format_type(_r_tag.text)
        g_int = parsing_utils.format_type(_g_tag.text)
        b_int = parsing_utils.format_type(_b_tag.text)
        alpha_int = parsing_utils.format_type(_alpha_tag.text)
        if isinstance(r_int, int) and \
           isinstance(g_int, int) and \
           isinstance(b_int, int) and \
           isinstance(alpha_int, int):
            return (r_int, g_int, b_int, alpha_int)
    print("Failed to parse a color tag")
    exit(1)

def parse_params(params_tag):
    """ Parses the ```Params``` tag from an element.
    The only expected param is the size. The other depends on the tag.
    """
    answer = {}

    size = params_tag.find('size')
    if size is None:
        parsing_utils.fail_not_found("Size in Params tag")
    answer.update(get_size(size))
    params_tag.remove(size)
    for child in list(params_tag):
        if 'color' in child.tag:
            answer.update({child.tag : parse_color(child)})
        else:
            answer.update({child.tag : parsing_utils.format_type(child.text)})
    return answer

def parse_element(element_tag):
    """ Parses an Element Field of the xml file."""
    name = element_tag.tag

    answer = {"name" : name}
    _params = element_tag.find("Params")

    if _params is None:
        parsing_utils.fail_not_found("Params")

    answer.update({"params" : parse_params(_params)})

    if "Text" in name:
        text = element_tag.find("text")
        if text is not None:
            answer.update({"text": text.text})

    elif "Button" in name:
        text = element_tag.find("text")
        if text is not None:
            answer.update({"text": text.text})
        action = element_tag.find("action")
        if action is not None:
            answer.update({"action": action.text})

    elif "Container" in name:
        _content = element_tag.find("Content")
        if _content:
            content = []
            for widget in list(_content):
                content.append(parse_element(widget))
            answer.update({"content": content})
    return answer

def parse_file(path):
    result = []
    root = parsing_utils.try_open_and_parse(path)
    result = parse_element(root)
    return result

