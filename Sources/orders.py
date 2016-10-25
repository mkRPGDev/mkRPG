from enum import IntEnum

from const import *

OrderType = IntEnum('OrderType', 'Set Create Destroy Move Activate')

class Order:
    def __init__(self):
        self.type = None
        self.args = []
        
    def load(self, dat):
        assert dat.name == "Order"
        if dat.args["type"] == "set":
            self.type = OrderType.Set
            self.args = [0]*3
            for nm, args, _ in dat.list:
                if nm == "target":  self.args[0] = args["val"]
                elif nm == "param": self.args[1] = args["val"]
                elif nm == "value": self.args[2] = args["val"]
        return self
    
    def toBytes(self):
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

if __name__=="__main__":
    o = Order()
    o.type = OrderType.Set
    o.args = ["me", "myself", "I"]
    b = o.toBytes()
    oo = Order()
    oo.fromBytes(b)
    print(b)
    print(oo.__dict__==o.__dict__)
