from enum import IntEnum

from utils import readXml

InteractionType = IntEnum("InteractionType", "Key Mouse Scroll Multi")

class Interaction():
    def __init__(self):
        self.target = None

    def load(self, dat):
        for d in dat.list:
            if d.name == "key":
                self.type = InteractionType.Key
                self.key = int(d.args["val"])
            elif d.name == "target":
                self.target = d.args["val"]
            elif d.name == "event":
                self.event = d.args["val"]
            elif d.name == "button":
                self.type = InteractionType.Mouse
                self.key = int(d.args["val"])
        return self

def registerInteractions(path):
    dat = readXml(path + "interactions.xml")
    assert dat.name == "Interactions"
    l = []
    for d in dat.list:
        l.append(Interaction().load(d))
    return l

if __name__=="__main__":
    registerInteractions("../Test/Snake/")
