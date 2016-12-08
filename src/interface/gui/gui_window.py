from plugin import PygamePlugin
from interface.cache import ImageCache

class GUIWindow:
    """ Representation of a gui window xml file """
    pass

class GUIWindowView(PygamePlugin):
    """ Viewer of a gui window, pygame client uses instances of this class """

    def __init__(self, guiElement, netPlugin):
        super().__init__(netPlugin)
        self.guiElement = guiElement

    def icon(self):
        return ImageCache.get_image(self.guiElement.icon)

    def draw(self):
        pass
