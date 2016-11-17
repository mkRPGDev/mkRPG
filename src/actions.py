from collections import namedtuple
import sys

sys.path.append("./parsing")

import parsing.global_parsing as global_parsing

Action = namedtuple("Action", "event orders conditions")

from utils import readXml
from orders import Order

# TODO enlever named
def loadAction(event, dat, named):
    """ Renvoie une instance d'action à partir d'une 
        structure et de la résolution des noms """
    orders = []
    for order in dat:
        print(order)
        orders.append(Order().load(order, named))
        print("Named list %s " % named)
    return Action(event, orders, [])

def registerActions(path, named):
    """ Créé une liste d'action à partir d'un Xml les décrivant """

    action_dict =  global_parsing.game_parser(path+"game.xml")["Actions"]

    actions = {}
    for action in action_dict:
        act = loadAction(action, action_dict[action], named)
        if action not in actions.keys():
            actions[action] = []
        actions[action].append(act)
    return actions

if __name__=="__main__":
    print(registerActions("../Test/Snake/")[0])
