"""
    This plugin adds buttons to change the viewpoint of the camera. Logical part
"""

from plugins.plugin import Plugin

class Mapcontrol(Plugin):
    """ MapControl plugin """
    def __init__(self, e):
        super().__init__(e)

    def rotate(self, step):
        """ Rotates the map (move camera horizontally) """
        self.engine.interface.mapView.rotate(step)

    def flatten(self, step):
        """ Flattens or de-flattens the map (move camera vertically) """
        self.engine.interface.mapView.flatten(step)
