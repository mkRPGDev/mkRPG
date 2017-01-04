"""
    This plugin adds buttons to change the viewpoint of the camera. Logical part
"""

from plugins.plugin import Plugin

class Mapcontrol(Plugin):
    """ MapControl plugin """
    def __init__(self, e):
        super().__init__(e)

    def rotate(self, step):
        """ rotate the map (move camera horizontally) """
        print("ROTATE !!")
        #self.engine.interface.mapView.rotate(step)

    def flatten(self, step):
        """ flatten or de-flatten the map (move camera vertically) """
        print("FLATTEN !!")
        #self.engine.interface.mapView.flatten(step)
