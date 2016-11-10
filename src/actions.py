from collections import namedtuple

Action = namedtuple("Action", "event orders conditions")

from tools import readXml
from orders import Order

# TODO enlever named
def loadAction(dat, named):
    assert dat.name == "Action"
    orders = []
    for d in dat.list:
        if d.name == "Event": ev = d.args["val"]
        elif d.name == "Order":
            orders.append(Order().load(d, named))
    return Action(ev, orders, [])

def registerActions(path, named):
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
