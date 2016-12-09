from plugin import PygamePlugin
from interface.cache import ImageCache

class GUIWindow:
    """ Representation of a gui window xml file """
    pass

class GUIWindowView(PygamePlugin):
    """ Viewer of a gui window, pygame client uses instances of this class """

    # TODO : Should build a single GUIElement (probably a
    #        gui_elements.Container) that represents the window given
    #        the GUIWindow representation.

    def __init__(self, guiWindow, netPlugin):
        super().__init__(netPlugin)
        self.guiElement = guiElement

    def icon(self):
        return ImageCache.get_image(self.guiElement.icon)

    def draw(self):
        pass
