"""
This module handles the orders.
Orders are described in the xml files, and are the actions to be executed
for example when keyboard keys are hitted, or in reaction to other orders.
Every order is thus initialized after the parsing fo the xml files
and the created in this file.

Each order is applied to a target called the emitter meaning that expressions
will be evaluated within this target context ie self.x will be target.x.
Without more precisions, when an event is raised by an order, its target is
transmitted and order generated be this event will share that target.

Set : target.param <- value
Setobj : target.param <- value and value is interpreted as an object
Timer : raise event in value ms
Event : raise event on the target target
Create : create an object of type base, execute the initcode (that allows
    transmitting fields of the target to the created object) and raise event on
    the created object - if an emitter is specified, it is used as a way to fix
    the identifier of the created object since the emitter can't exist
Destroy : destroy the emitter
Condition : raise event if value is True
Move : param being a list (quests for example) move the emitter from 
    source.param to dest.param
Watchdog : start looking after target.param, when it changes to value (that was
    evaluated once by the time of the order), it raises the event
    if once is True, the watchdog is destroy after the first event happened
"""

from collections import namedtuple
from enum import IntEnum

from shared.const import CODING

ORDERTYPE = IntEnum('OrderType', 'Set Timer Event Create Destroy Condition '
                                 'Move Setobj Watchdog')
Condition = namedtuple("Condition", "target event once")

class Order:
    """ A change to be done on the world"""
    # Attention aux collisions avec args et type
    params = [None] * (len(ORDERTYPE)+1) #XXX c'pas top
    params[ORDERTYPE.Set] = ["target", "param", "value"]
    params[ORDERTYPE.Timer] = ["event", "value"]
    params[ORDERTYPE.Event] = ["event", "target"]
    params[ORDERTYPE.Create] = ["event", "base", "init"]
    params[ORDERTYPE.Destroy] = []
    params[ORDERTYPE.Condition] = ["event", "value"]
    params[ORDERTYPE.Move] = ["source", "dest", "param"]
    params[ORDERTYPE.Setobj] = ["target", "param", "value"]
    params[ORDERTYPE.Watchdog] = ["target", "param", "value", "event", "once"]

    def __init__(self):
        self.type = None
        self.args = []

    def __getattr__(self, attr):
        return self.args[self.params[self.type].index(attr)]

    def __setattr__(self, attr, val):
        if attr in ("type", "args") or attr not in self.params[self.type]:
            object.__setattr__(self, attr, val)
        else:
            self.args[self.params[self.type].index(attr)] = val

    def copy(self):
        """ Copy the object from class Order """
        # une copy.deepcopy aurait copié params
        obj = Order()
        obj.type = self.type
        obj.args = self.args[:]
        return obj

    def setType(self, typ):
        """ Initialise args according to the given type """
        self.type = typ
        self.args = [None]*len(self.params[self.type])

    def load(self, dat, named):
        """ Initialise the order with an Xml structure """
        self.setType(ORDERTYPE.__members__[dat["type"].capitalize()])
        for key in dat.keys():
            if key != 'type':
                if isinstance(dat[key], dict) and dat[key].get("id") is not None:
                    self.args[self.params[self.type].index(key)] =\
                    str(named[dat[key]['id']].ident)
                else:
                    self.args[self.params[self.type].index(key)] = dat[key]
        return self

    def toBytes(self): # TODO éliminer tt les str => ids de param
        """ Bytes to send the order on the network """
        def addStr(s):
            assert len(s) < 1<<16
            byt.extend(len(s).to_bytes(2, 'big'))
            byt.extend(s.encode(CODING))
        byt = bytearray()
        byt.append(self.type)
        for arg in self.args: addStr(arg)
        return bytes(byt)

    def fromBytes(self, byt):
        """ Retrieve order from network bytes """
        def getStr():
            nonlocal i
            l = int.from_bytes(byt[i:i+2], 'big')
            s = byt[i+2 : i+2+l].decode(CODING)
            i += l+2
            return s
        self.type = byt[0]
        i = 1
        self.args = [getStr() for _ in range(len(self.params[self.type]))]
        assert self.toBytes() == byt[:i]
        return self, i

class OrderDispatcher:
    """ Treat orders for client and server """
    def __init__(self, world, handle, timer):
        self.world = world
        self.handle = handle
        self.timer = timer

    async def treat(self, emitter, order):
        """ Treat an order and return an order to retransmit if any """
        world = self.world
        try:
            if order.type == ORDERTYPE.Set:
                target = emitter if order.target == "emitter" else eval(order.target)
                try:
                    val = target.contextEval(order.value)
                except:
                    print(emitter.case)
                    raise
                preval = target.params[order.param]
                if val != preval:
                    target.params[order.param] = val
                    returnOrder = order.copy()
                    returnOrder.value = str(val)
                    for condition in target.conditions[order.param][val]:
                        await self.handle(condition.target, condition.event)
                    # XXX pas fameux
                    target.conditions[order.param][val] = \
                        list(filter(lambda x: not x.once,
                                    target.conditions[order.param][val]))
                    if not target.conditions[order.param][val]:
                        del target.conditions[order.param][val]
                    return returnOrder
                return None
            if order.type == ORDERTYPE.Timer:
                # les Timer transmettent leur contexte
                if emitter:
                    self.timer.add(emitter.contextEval(order.value), self.handle,
                                   args=[emitter, order.event])
                else:
                    self.timer.add(int(order.value), self.handle,
                                   args=[emitter, order.event])
                return None
            if order.type == ORDERTYPE.Event:
                if order.target:
                    await self.handle(eval('emitter.'+order.target), order.event)
                else:
                    await self.handle(emitter, order.event)
                return None
            if order.type == ORDERTYPE.Create:
                new = world.ids[int(order.base)]
                if isinstance(emitter, int):
                    obj = new.create(emitter)
                    world.numid[0] = emitter
                else:
                    obj = new.create()
                self.world.objects.append(obj)
                exec(order.init)
                if self.handle:
                    await self.handle(obj, order.event)
                return order
            if order.type == ORDERTYPE.Destroy:
                # TODO nécessite de trouver tous les pointeurs ??
                self.world.objects.remove(emitter)
                self.world.ids.pop(emitter.ident)
                return order
            if order.type == ORDERTYPE.Condition:
                if emitter.contextEval(order.value):
                    await self.handle(emitter, order.event)
                return None
            if order.type == ORDERTYPE.Move:
                eval(order.source+"."+order.param).remove(emitter)
                eval(order.dest+"."+order.param).append(emitter)
                return order
            if order.type == ORDERTYPE.Setobj: # TODO à améliorer ressemble à Set
                # FIXME plante avec un aléa
                target = emitter if order.target == "emitter" else eval(order.target)
                val = target.contextEval(order.value)
                preval = eval("target."+order.param)
                if val != preval:
                    exec("target."+order.param+"=val")
                    return order
                return None
            if order.type == ORDERTYPE.Watchdog:
                val = emitter.contextEval(order.value)
                conds = eval(order.target).conditions[order.param][val]
                conds.append(Condition(emitter, order.event, order.once))
                return None
        except:
            print("An order failed !")
            print(emitter)
            print(order.type)
            print(order.args)
            raise
