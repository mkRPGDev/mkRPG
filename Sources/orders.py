from enum import IntEnum
from threading import Timer

from const import *

OrderType = IntEnum('OrderType', 'Set Timer Event Create Destroy Move Activate')

class Order:
    params = [None] * len(OrderType)
    params[OrderType.Set] = ["target", "param", "value"]
    params[OrderType.Timer] = ["event", "value"]
    params[OrderType.Event] = ["event", "target"]

    def __init__(self):
        self.type = None
        self.args = []
    
    def __getattribute__(self, attr):
        try:
            return super().__getattribute__(attr)
        except AttributeError:
            if attr in self.params[self.type]:
                return self.args[self.params[self.type].index(attr)]
            raise

    def __setattr__(self, attr, val):
        try:
            super().__setattr__(attr, val)
        except AttributeError:
            if attr in self.params[self.type]:
                self.args[self.params[self.type].index(attr)] = val
            raise
    
    def load(self, dat):
        assert dat.name == "Order"
        self.type = OrderType.__members__[dat.args["type"].capitalize()]
        self.args = [0]*len(self.params[self.type])
        for nm, args, _ in dat.list:
            self.args[self.params[self.type].index(nm)] = args["val"]
        return self
    
    def toBytes(self): # TODO éliminer tt les str => ids de param
        def addStr(s):
            assert len(s) < 665536
            b.append(len(s)//256)
            b.append(len(s)%256)
            b.extend(s.encode(CODING))
        b = bytearray()
        b.append(self.type)
        if self.type == OrderType.Set:
            for arg in self.args:
                addStr(arg)
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
        if self.type == OrderType.Set:
            self.args = [getStr() for _ in range(3)]
        return self
    
    def dispatch(self, context):
        pass

class OrderDispatcher:
    """ pour diminuer la redondance de code client/serveur """
    def __init__(self, world, handle):
        self.world = world
        self.handle = handle

    def treat(self, emitter, order):
        """ -> transmission nécessaire """
        if order.type==OrderType.Set:
            if order.target=="emitter":
                emitter.treatOrder(order)
            elif order.target=="world":
                self.world.treatOrder(order)
            return True
        if order.type==OrderType.Timer:
            # les Timer transmettent leur contexte
            Timer(float(order.value), self.handle, args=[emitter, order.event]).start()
            return False
        if order.type==OrderType.Event:
            self.handle(eval('emitter.'+order.target), order.event)
            return False

if __name__=="__main__":
    o = Order()
    o.type = OrderType.Set
    o.args = ["me", "myself", "I"]
    b = o.toBytes()
    oo = Order()
    oo.fromBytes(b)
    print(b)
    print(oo.__dict__==o.__dict__)
