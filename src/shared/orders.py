from collections import namedtuple
from enum import IntEnum

from const import *

OrderType = IntEnum('OrderType', 'Set Timer Event Create Destroy Condition '
                                 'Move Setobj Watchdog')
Condition = namedtuple("Condition", "target event once")

class Order:
    """ Représente une modification à apporter au monde """
    # Attention aux collisions avec args et type
    params = [None] * (len(OrderType)+1) #XXX c'pas top 
    params[OrderType.Set] = ["target", "param", "value"]
    params[OrderType.Timer] = ["event", "value"]
    params[OrderType.Event] = ["event", "target"]
    params[OrderType.Create] = ["event", "base", "init"]
    params[OrderType.Destroy] = []
    params[OrderType.Condition] = ["event", "value"]
    params[OrderType.Move] = ["source", "dest", "param"]
    params[OrderType.Setobj] = ["target", "param", "value"]
    params[OrderType.Watchdog] = ["target", "param", "value", "event", "once"]

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
        # une copy.deepcopy aurait copié params
        obj = Order()
        obj.type = self.type
        obj.args = self.args[:]
        return obj
    
    def load(self, dat, named):
        """ Initialise l'ordre avec une structure provenant d'un Xml """
        self.type = OrderType.__members__[dat['type'].capitalize()]
        self.args = [0]*len(self.params[self.type])
        for key in dat.keys():
            if key != 'type':
                if type(dat[key]) == dict and dat[key].get("id") is not None:
                    self.args[self.params[self.type].index(key)] = str(named[(dat[key]["id"])].ident)
                else:
                    self.args[self.params[self.type].index(key)] = dat[key]
#        for nm, args, _ in dat.list:
#            if "val" in args:
#                self.args[self.params[self.type].index(nm)] = args["val"]
#            else:
#                # TODO utiliser des pointeurs
#                self.args[self.params[self.type].index(nm)] = \
#                    str(named[args["id"]].ident)
        return self
    
    def toBytes(self): # TODO éliminer tt les str => ids de param
        """ Bytes pour envoyer l'ordre sur le réseau """
        def addStr(s):
            assert len(s) < 1<<16
            b.append(len(s)//256)
            b.append(len(s)%256)
            b.extend(s.encode(CODING))
        b = bytearray()
        b.append(self.type)
        for arg in self.args: addStr(arg)
        return bytes(b)
        
    def fromBytes(self, b):
        """ Récupère l'ordre à partir d'un bytes réseau """
        def getStr():
            nonlocal i
            l = 256*b[i] + b[i+1]
            s = b[i+2 : i+2+l].decode(CODING)
            i += l+2
            return s
        self.type = b[0]
        i = 1
        self.args = [getStr() for _ in range(len(self.params[self.type]))]
        return self, i

class OrderDispatcher:
    """ Traite les ordres pour le client ou le serveur """
    def __init__(self, world, handle, timer):
        self.world = world
        self.handle = handle
        self.timer = timer

    async def treat(self, emitter, order):
        """ Traite un ordre et renvoie l'éventuel ordre à retransmettre """
        world = self.world
        if order.type==OrderType.Set:
            target = emitter if order.target=="emitter" else eval(order.target)
            val = target.contextEval(order.value)
            preval = target.params[order.param]
            if val!=preval:
                target.params[order.param] = val
                returnOrder = order.copy()
                returnOrder.value = str(val)
                for condition in target.conditions[order.param][val]:
                    await self.handle(condition.target, condition.event)
                # XXX pas fameux
                target.conditions[order.param][val] = \
                    list(filter(lambda x:not x.once, target.conditions[order.param][val]))
                if not target.conditions[order.param][val]:
                    del target.conditions[order.param][val]
                return returnOrder
            return None
        if order.type==OrderType.Timer:
            # les Timer transmettent leur contexte
            if emitter:
                self.timer.add(emitter.contextEval(order.value), self.handle,
                               args=[emitter, order.event])
            else:
                self.timer.add(int(order.value), self.handle,
                                     args=[emitter, order.event])
            return None
        if order.type==OrderType.Event:
            if order.target:
                await self.handle(eval('emitter.'+order.target), order.event)
            else:
                await self.handle(emitter, order.event)
            return None
        if order.type==OrderType.Create:
            obj = self.world.ids[int(order.base)].create()
            self.world.objects.append(obj)
            exec(order.init)
            if self.handle:
                await self.handle(obj, order.event)
            return order
        if order.type==OrderType.Destroy:
            # TODO nécessite de trouver tous les pointeurs ??
            self.world.objects.remove(emitter)
            self.world.ids.pop(emitter.ident)
            return order
        if order.type==OrderType.Condition:
            if emitter.contextEval(order.value):
                await self.handle(emitter, order.event)
            return None
        if order.type==OrderType.Move:
            eval(order.source+"."+order.param).remove(emitter)
            eval(order.dest+"."+order.param).append(emitter)
            return order
        if order.type==OrderType.Setobj: # TODO à améliorer ressemble à Set
            # FIXME plante avec un aléa
            target = emitter if order.target=="emitter" else self.world
            val = target.contextEval(order.value)
            preval = eval("target."+order.param)
            if val!=preval:
                exec("target."+order.param+"=val")
                return order
            return None
        if order.type==OrderType.Watchdog:
            val = emitter.contextEval(order.value)
            conds = eval(order.target).conditions[order.param][val]
            conds.append(Condition(emitter, order.event, order.once))
            return None
