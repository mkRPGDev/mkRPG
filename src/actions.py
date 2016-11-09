from collections import namedtuple

Action = namedtuple("Action", "event orders conditions")

from utils import readXml
from orders import Order

# TODO enlever named
def loadAction(dat, named):
    """ Renvoie une instance d'action à partir d'une 
        structure et de la résolution des noms """
    assert dat.name == "Action"
    orders = []
    for d in dat.list:
        if d.name == "Event": ev = d.args["val"]
        elif d.name == "Order":
            orders.append(Order().load(d, named))
    return Action(ev, orders, [])
        
def registerActions(path, named):
    """ Créé une liste d'action à partir d'un Xml les décrivant """
    dat = readXml(path + "actions.xml")
    assert dat.name == "Actions"
    actions = {}
    for d in dat.list:
        act = loadAction(d, named)
        if act.event not in actions:
            actions[act.event] = []
        actions[act.event].append(act)
    return actions

if __name__=="__main__":
    print(registerActions("../Test/Snake/")[0])
