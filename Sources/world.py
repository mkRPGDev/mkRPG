from enum import IntEnum
import os

from isserver import SERVER
from utils import readXml
from orders import Order, OrderType
#l'autre solution est de tout mettre dans une fonction 
# qui écrasera des classes bidon en global

verbose = False

named = {}
toResolve = []

def loadGame(path):
    dat = readXml(path + "game.xml")
    assert dat.name == "Game"
    lw = []
    for d in dat.list:
        if d.name == "World":
            lw.extend(d.list)
    for f in lw:
        dat = readXml(path+f.args["file"])
        eval(dat.name)().subload(dat)
    w = named["world"]
    for m in w.maps: m.fill()
    return w

class BaseObject:
    ident = 0
    ids = {} # liste si sans deletion, RIP fetch

    def __init__(self):
        BaseObject.ident += 1
        BaseObject.ids[BaseObject.ident] = self
        self.ident = BaseObject.ident
        self.params = {}
    
    def __getattribute__(self, attr):
        try:
            return super().__getattribute__(attr)
        except AttributeError:
            if attr in self.params:
                return self.params[attr]
            raise
    
    def load(self, path):
        if verbose: print(path)
        dirs = os.listdir(path)
        for nm, ty in self.__dict__.items():
            if type(ty) is not list or nm not in dirs: continue
            #C = eval(nm[:-1].capitalize()) # TODO trouver mieux
            C = evl[nm]# sapristi
            assert(type(C)==type)
            for m in os.listdir(path+nm):
                obj = C() 
                obj.load(path+nm+"/"+m+"/")
                ty.append(obj)
        data = readXml(path + self.__class__.__name__.lower() + ".xml")
        self.subload(data) # en bas pour l'héritage
        
    def subload(self, data):
        if verbose: print(data.name)
        #assert(data.name == self.__class__.__name__)
        for d in data.list:
            n = d.name
            if n=="Params": #peut gérer plusieurs def de params
                for np, ap, _ in d.list:
                    if "val" in ap:
                        self.params[np] = int(ap["val"]) # on a supposé un int
                    else:
                        toResolve.append((ap["id"], self.params, np))#, True))
            elif n==self.__class__.__name__+"Type": # ObjectType
                pass
            elif (n.lower() in self.__dict__ and 
                 type(self.__dict__[n.lower()])==list):
                li = self.__dict__[n.lower()]
                C = evl[n.lower()]
                for dat in d.list:
                    if "id" in dat.args:
                        toResolve.append((dat.args["id"], li, len(li)))#, False))
                        li.append(None)
                    else:
                        li.append(C().subload(dat))
            elif n.endswith("Type") and type(eval(n[:-4]))==type and "name" in d.args:
                ObjectType(eval(n[:-4])).subload(d)
            elif type(eval(n))==type and "name" in d.args:
                eval(n)().subload(d)
                
        if "name" in data.args:
            named[data.args["name"]] = self
            #return data.args["name"]
        for nm, li, ln in toResolve: #TODO a opti
            if nm not in named: continue
            if verbose: print(nm, "->", named[nm])
            li[ln] = named[nm]
            #assert nm in named, nm+" non résolu" FIXME
#            if bo: li[ln] = named[nm].ident # dico
#            else:  li[ln] = named[nm]       # liste
        return self
    
    def treatOrder(self, order): #TODO traitement formules
        if order.type == OrderType.Set:
            if verbose: print(self.__class__.__name__, order.param, "changed", order.value)
            #self.params[order.param] = eval(order.value)
            # FIXME pour résoudre les . (entities[0].x)
            exec("self."+order.param+"="+order.value)
        else:
            raise NotImplemented
#        if order.type == OrderType.Add:
#            self.prop[order.arg1] += order.arg2
#        if order.arg1 in self.cond:
#            [self.engine.triggerEvent(i) for i in self.cond[order.arg1]]
#        else:
#            self.specialTreatment(order)

class ServerObject(BaseObject):
    def __init__(self):
        BaseObject.__init__(self)
    
#    def treatOrder(self, order):
#        pass

class ClientObject(BaseObject):
    def __init__(self):
        BaseObject.__init__(self)
        self.image = None
    
#    def buildEvent(self):
#        pass


MagicObject = ServerObject if SERVER else ClientObject
#pour éviter la confusion avec object

class ObjectType(MagicObject):
    def __init__(self, typ = MagicObject):
        super().__init__()
      #  self.defParams = {} #TODO plage pb de synchro
        self.type = typ
    
    def create(self):
        instance = self.type()
        for p,v in self.params.items():
            instance.params[p] = v
        return instance
    
#    def load(self, path):
#        raise NotImplemented

class World(MagicObject):
    def __init__(self):
        MagicObject.__init__(self)
        self.maps = [] # une liste c'est mieux non ?
        self.entities = []
        self.objects = []
    
        
class Map(MagicObject):
    def __init__(self):
        MagicObject.__init__(self)
        self.cells = []
        
       #! load et subload    
#    def subload(self, data):
#        super().subload(data)
        
    def fill(self):
        self.cellsGrid = [[None] * self.height
                            for _ in range(self.width)]
        for c in self.cells:
            self.cellsGrid[c.x][c.y] = c
        for i,l in enumerate(self.cellsGrid):
            for j,e in enumerate(l):
                if not e: 
                    cell = BaseObject.ids[self.defaultCell].create()
                    l[j] = cell
                    self.cells.append(cell)
                    cell.x = i; cell.y = j
    
#    def computeLOV(self, pos):
#        pass

class Cell(MagicObject):
    def __init__(self):
        MagicObject.__init__(self)
        self.entities = []
        self.objects = []
        
class Entity(MagicObject):
    def __init__(self):
        MagicObject.__init__(self)
        self.quests = [] # so noob Robert
        self.inventory = []
    
#    def load(self, dic):
#        MagicObject.load(self, dic)
#        for n, _, l in dic.list:
#            if n=="Baluchon": #peut gérer plusieurs def de params
#                for np, ap, _ in l:
#                    self.inventory.append()
#        

evl = {"maps":Map, "entities":Entity, "cells":Cell, "objects":MagicObject,
       "types":ObjectType, "inventory":MagicObject, "quests":MagicObject}


if __name__=="__main__":
    verbose = True
    w = World()
    w.load("../Test/")

