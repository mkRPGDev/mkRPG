""" This module creates a view of the User Interface.
It relies on the gui_elements module, and the parsing module in order to create
the view."""

from plugins.plugin import PygamePlugin
from interface.cache import ImageCache
from interface.const import DEFAULT_GUI_VALUES
from parsing import gui_parser



class GUIWindow:
    """ Representation of a gui window xml file """
    def __init__(self, file_path):
        """ Parses a xml file describing the user interface.
        It also sets up default values.
        """
        data = gui_parser.parse_file(file_path)
        if data.get('name') != 'Container':
            # The file doesn't describe a container. Exits the run.
            print("Invalid file : The first tag isn't a container !")
            exit(1)
        self.name = data['name']
        _params = data.get('params')
        if _params is None:
            # Safety check. Shouldn't happen since it should have been checked
            # In the parsing
            print("Invalid file : No params for the container.")
            exit(1)
        self.params = _params
        _content = data.get('content')
        if _content is None:
            # Safety check. Shouldn't happen since it should have been checked
            # In the parsing
            print("Invalid file : No content for the container.")
            exit(1)
        self.content = _content

    def _set_defaul_value(self, widget, param):
        """ Sets the default value for the given param.
            - ```param``` should be a string."""
        assert isinstance(param, str)
        if self.params.get(param) is None:
            try:
                value = DEFAULT_GUI_VALUES[widget][param]
                self.params[param] = value
            except AttributeError:
                raise AttributeError("No default value for parameter %s" %
                                     param)


    def _set_content_default_values(self, content, param):
        """ Sets the default value for the given content param.
        - ```param``` should be a string, ```content``` the index of the
          content in the contents list.
        """
        assert isinstance(param, str)
        assert isinstance(content, int)
        _content = self.content[content]
        if _content is not None:
            # No need to check the name, it has been done previously.
            try:
                value = (DEFAULT_GUI_VALUES[_content]["name"])[param]
                self.content[content][param] = value
            except AttributeError:
                raise AttributeError(
                    "No default value for parameter %s in content %s " %
                    (param, content)
                    )

    def set_default_values(self):
        """ Sets all default values of the object. """
        for param in self.params:
            self._set_defaul_value(self.name, param)
        for (index, content) in enumerate(self.content):
            if content.get('name') is not None:
                for param in DEFAULT_GUI_VALUES[content]["name"]:
                    self._set_content_default_values(index, param)


class GUIWindowView(PygamePlugin):
    """ Viewer of a gui window, pygame client uses instances of this class """

    # TODO : Should build a single GUIElement (probably a
    #        gui_elements.Container) that represents the window given
    #        the GUIWindow representation.

    def __init__(self, guiWindow, netPlugin):
        super().__init__(netPlugin)
        self.guiWindow = guiWindow
        self.guiElements = make_elements(guiWindow)

    def make_elements(self, guiWindow):
        # TODO : Build a list of guiElements
        pass

    def icon(self):
        return ImageCache.get_image(self.guiElement.icon)

    def draw(self):
        pass
