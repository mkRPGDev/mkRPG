from plugins.plugin import Plugin

class Mapcontrol(Plugin):
    def __init__(self, e):
        super().__init__(e)

    def rotate(self, step):
        self.engine.interface.mapView.rotate(step)

    def flatten(self, step):
        self.engine.interface.mapView.flatten(step)
