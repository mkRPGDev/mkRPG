from collections import namedtuple

Action = namedtuple("Action", "event orders conditions")

from shared.tools import readXml
from shared.orders import Order
from parsing import global_parsing

# XXX pas tout à fait partagé mais temporaire

# TODO enlever named
def loadAction(event, dat, named):
    """ Returns an action instance from a Xml structure and name mapping """
    orders = []
    for order in dat:
        orders.append(Order().load(order, named))
    return Action(event, orders, [])

def registerActions(path, named):
    """ Create a list of action from a Xml describing them """
    action_dict =  global_parsing.game_parser(path + 'game.xml')['Actions']
    actions = {}
    for action in action_dict:
        act = loadAction(action['event'], action['orders'], named)
        if action['event'] not in actions.keys():
            actions[action['event']] = []
        actions[action['event']].append(act)
    return actions

if __name__=="__main__":
    print(registerActions("../Test/Snake/")[0])
