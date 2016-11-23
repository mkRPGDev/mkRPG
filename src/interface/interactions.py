from enum import IntEnum

from shared.tools import readXml

InteractionType = IntEnum("InteractionType", "Key Mouse Scroll Multi")

class Interaction():
    """ Représente une connection entre une entrée utilisateur 
        et un événement à déclencher sur une cible """
    def __init__(self):
        self.target = None

    def load(self, dat):
        self.type = InteractionType.Key
        self.key = dat["key"]
        self.target = dat["target"]
        self.event = dat["event"]
        return self

def registerInteractions(path):
    """ Créé une liste d'interaction à partir d'un Xml les décrivant """
    interactions_list = global_parsing.game_parser(path+"game.xml")["Interactions"]
    l = []
    for d in dat.list:
        l.append(Interaction().load(d))
    return l

if __name__=="__main__":
    registerInteractions("../Test/Snake/")
