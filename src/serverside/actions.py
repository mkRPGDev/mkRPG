from collections import namedtuple

Action = namedtuple("Action", "event orders conditions")

from shared.tools import readXml
from shared.orders import Order

# XXX pas tout à fait partagé mais temporaire

# TODO enlever named
def loadAction(dat, named):
    """ Returns an action instance from a Xml structure and name mapping """
    assert dat.name == "Action"
    orders = []
    for d in dat.list:
        if d.name == "Event": ev = d.args["val"]
        elif d.name == "Order":
            orders.append(Order().load(d, named))
    return Action(ev, orders, [])

def registerActions(path, named):
    """ Create a list of action from a Xml describing them """
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
