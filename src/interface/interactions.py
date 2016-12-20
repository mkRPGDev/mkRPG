from enum import IntEnum

from shared.tools import readXml
from parsing import global_parsing

InteractionType = IntEnum("InteractionType", "Key Mouse Scroll Multi")

class Interaction():
    """ Link between an user input and a event to trigger on a target """
    def __init__(self):
        self.target = None

    def load(self, dat):
        if dat.get("key"):
            self.type = InteractionType.Key
            self.key = dat["key"]
        elif dat.get("mouse"):
            self.type = InteractionType.Mouse
            self.key = dat["mouse"]
        self.target = dat["target"]
        self.event = dat["event"]
        return self

def registerInteractions(path):
    """ Create a list of interaction from a Xml describing them """
    interactions_list = global_parsing.game_parser(path+"game.xml")["Interactions"]
    l = []
    for d in interactions_list:
        l.append(Interaction().load(d))
    return l

if __name__=="__main__":
    registerInteractions("../Test/Snake/")
