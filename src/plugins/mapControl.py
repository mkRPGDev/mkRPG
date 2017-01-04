from plugins.plugin import Plugin

class Mapcontrol(Plugin):
    def __init__(self, e):
        super().__init__(e)

    def rotate(self, step):
        print("ROTATE !!")
        #self.engine.interface.mapView.rotate(step)

    def flatten(self, step):
        print("FLATTEN !!")
        #self.engine.interface.mapView.flatten(step)
