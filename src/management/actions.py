""" Hosts the actions and a function to create them """

from collections import namedtuple
from shared.orders import Order

Action = namedtuple("Action", "event orders conditions")

def loadAction(event, dat, named):
    """ Returns an action instance from a Xml structure and name mapping """
    orders = []
    for order in dat:
        orders.append(Order().load(order, named))
    return Action(event, orders, [])

def registerActions(action_dict, named):
    """ Create a list of action from a Xml describing them """
    actions = {}
    for action in action_dict:
        act = loadAction(action['event'], action['orders'], named)
        if action['event'] not in actions.keys():
            actions[action['event']] = []
        actions[action['event']].append(act)
    return actions

