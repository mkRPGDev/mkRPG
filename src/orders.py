from enum import IntEnum
#from threading import Timer
from utils import Timer

from const import *

OrderType = IntEnum('OrderType', 'Set Timer Event Create Destroy Condition '
                                 'Move Setobj Watchdog')

class Order:
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
        assert dat.name == "Order"
        self.type = OrderType.__members__[dat.args["type"].capitalize()]
        self.args = [0]*len(self.params[self.type])
        for nm, args, _ in dat.list:
            if "val" in args:
                self.args[self.params[self.type].index(nm)] = args["val"]
            else:
                # TODO utiliser des pointeurs
                self.args[self.params[self.type].index(nm)] = \
                    str(named[args["id"]].ident)
        return self
    
    def toBytes(self): # TODO éliminer tt les str => ids de param
        def addStr(s):
            assert len(s) < 665536
            b.append(len(s)//256)
            b.append(len(s)%256)
            b.extend(s.encode(CODING))
        b = bytearray()
        b.append(self.type)
        for arg in self.args: addStr(arg)
        return bytes(b)
        
    def fromBytes(self, b):
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
    """ pour diminuer la redondance de code client/serveur """
    def __init__(self, world, handle):
        self.world = world
        self.handle = handle
        self.timer = Timer()
        self.timer.start()

    def treat(self, emitter, order):
        """ -> ordre à retransmettre """
        world = self.world
        if order.type==OrderType.Set:
            target = emitter if order.target=="emitter" else eval(order.target)
            val = target.contextEval(order.value)
            preval = target.params[order.param]#eval("target."+order.param)
            if val!=preval:
                # FIXME
                #exec("target."+order.param+"=val")
                target.params[order.param] = val
                returnOrder = order.copy()
                returnOrder.value = str(val)
                for eventTarget, event, once in target.conditions[order.param][val]:
                    self.handle(eventTarget, event) # TODO gérér le once
                return returnOrder
            return None
        if order.type==OrderType.Timer:
            # les Timer transmettent leur contexte
            if emitter:
                self.timer.add(emitter.contextEval(order.value), self.handle, args=[emitter, order.event])
            else:
                self.timer.add(int(order.value), self.handle, args=[emitter, order.event])
            return None
        if order.type==OrderType.Event:
            if order.target:
                self.handle(eval('emitter.'+order.target), order.event)
            else:
                self.handle(emitter, order.event)
            return None
        if order.type==OrderType.Create:
            obj = self.world.ids[int(order.base)].create()
            self.world.objects.append(obj)
            exec(order.init)
            if self.handle:
                self.handle(obj, order.event)
            return order
        if order.type==OrderType.Destroy:
            # TODO nécessite de trouver tous les pointeurs ??
            self.world.objects.remove(emitter)
            return order
        if order.type==OrderType.Condition:
            if emitter.contextEval(order.value):
                self.handle(emitter, order.event)
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
            eval(order.target).conditions[order.param][val].append((emitter, order.event, order.once))
            return None
                       
if __name__=="__main__":
    o = Order()
    o.type = OrderType.Set
    o.args = ["me", "myself", "I"]
    b = o.toBytes()
    oo = Order()
    oo.fromBytes(b)
    print(b)
    print(oo.__dict__==o.__dict__)
