from enum import IntEnum
from collections import defaultdict
from random import randint

from shared.tools import readXml
from shared.orders import Order, OrderType

verbose = False

named = {}
toResolve = []

world = None

def loadGame(path):
    """ Load game.xml and returns the loaded world """
    global world
    dat = readXml(path + "game.xml")
    assert dat.name == "Game"
    lw = []
    for d in dat.list:
        if d.name == "World":
            lw.extend(d.list)
    for f in lw:
        dat = readXml(path+f.args["file"])
        eval(dat.name)().load(dat)
    world = named["world"]
    for m in world.maps: m.fill()
    return world

class BaseObject:
    """ Any world object """
    ident = 0
    ids = {} # liste si sans deletion

    def __init__(self):
        BaseObject.ident += 1
        BaseObject.ids[BaseObject.ident] = self
        self.params = {} # Ne pas déplacer =)
        self.ident = BaseObject.ident

        self.conditions = defaultdict(lambda:defaultdict(list)) #TODO à déplacer

    def __getattr__(self, attr):
        if attr in self.params:
            return self.params[attr]
        raise AttributeError(attr)

    def __setattr__(self, attr, val):
        if attr is "params" or attr not in self.params:
            object.__setattr__(self, attr, val)
        else:
            self.params[attr] = val

    def load(self, data):
        """ Charge l'objet depuis une structure Xml """
        if verbose: print(data.name)
        for d in data.list:
            n = d.name
            if n=="Params": #peut gérer plusieurs def de params
                for np, ap, _ in d.list:
                    if "val" in ap:
                        self.params[np] = int(ap["val"]) # on a supposé un int
                    else:
                        toResolve.append((ap["id"], self.params, np))
            elif n==self.__class__.__name__+"Type": # ObjectType
                pass
            elif (n.lower() in self.__dict__ and
                 type(self.__dict__[n.lower()])==list):
                li = self.__dict__[n.lower()]
                C = plurals[n.lower()]
                for dat in d.list:
                    if "id" in dat.args:
                        toResolve.append((dat.args["id"], li, len(li)))
                        li.append(None)
                    else:
                        li.append(C().load(dat))
            elif n.endswith("Type") and type(eval(n[:-4]))==type and "name" in d.args:
                ObjectType(eval(n[:-4])).load(d)
            elif type(eval(n))==type and "name" in d.args:
                eval(n)().load(d)

        if "name" in data.args:
            named[data.args["name"]] = self
        for nm, li, ln in toResolve: #TODO a optimiser
            if nm not in named: continue
            if verbose: print(nm, "->", named[nm])
            li[ln] = named[nm]
            #assert nm in named, nm+" non résolu" FIXME
        return self

    def contextEval(self, value):
        """ Eval an expression in the context of the object for orders """ 
        return eval(value)

    # TODO traitement d'ordres ?

#class ServerObject(BaseObject): pass
#class ClientObject(BaseObject): pass

MagicObject = BaseObject
# ServerObject if SERVER else ClientObject
# pour éviter la confusion avec object

class ObjectType(MagicObject):
    """ Allowing to create objects with default parameters """
    def __init__(self, typ = MagicObject):
        super().__init__()
        self.type = typ

    def create(self):
        """ Instanticiation of an object from the type """
        instance = self.type()
        instance.type = self
        for p,v in self.params.items():
            instance.params[p] = v
        return instance

    def __str__(self):
        return str(self.params)

class World(MagicObject):
    """  """
    def __init__(self):
        MagicObject.__init__(self)
        self.maps = [] # une liste c'est mieux non ?
        self.entities = []
        self.objects = []

class Map(MagicObject):
    """ Orthonormed map with associated alogrithms """
    def __init__(self):
        MagicObject.__init__(self)
        self.cells = []

    def fill(self):
        """ Fill the cellsGrid attribute with cells and default cell
        To be called after the Xml reading only """
        self.cellsGrid = [[None] * self.height
                            for _ in range(self.width)]
        for c in self.cells:
            self.cellsGrid[c.x][c.y] = c
        for i,l in enumerate(self.cellsGrid):
            for j,e in enumerate(l):
                if not e:
                    cell = self.defaultCell.create()
                    l[j] = cell
                    self.cells.append(cell)
                    cell.x = i; cell.y = j
    
    def dist(self, source, dest):
        """ Manhattan distance """
        return abs(source.x-dest.x) + abs(source.y-dest.y)
    
    def lov(self, source, dest):
        """ Computes if the view is degaged between source and dest
        those laters must have x and y parameters and the map
        cells a parameter "visible"
        About O(dist(source, dest))
        """
        x1,y1 = source.x, source.y
        x2,y2 = dest.x, dest.y
        if x1>x2: x1,x2 = -x1,-x2
        if y1>y2: y1,y2 = -y1,-y2
        start = y1
        for x in range(x1,x2+1):
            seen = False
            for y in range(start,y2+1):
                # facteurs 2 pour rester sur des ints
                if (2*x-1-2*x1)*(y2-y1) < (2*y+1-2*y1)*(x2-x1) and \
                   (2*x+1-2*x1)*(y2-y1) > (2*y-1-2*y1)*(x2-x1):
                    if self.cellsGrid[abs(x)][abs(y)].visible:
                        return False
                    seen = True
                elif seen: break
                else: start = y
        return True

    def fromPos(self, pos, maxi=None):
        """ Yield cells further and further from pos, stopping at maxi
        Assuming x goes right and y down it turns as the trigo circle,
        it is therefore not a "serpentin" (sadly ?) """
        x,y = pos.x, pos.y
        if maxi is None: maxi = max(x, self.width-x) + max(y, self.height-y)
        for d in range(maxi+1):
            for dx,dy in ((-1,-1), (-1, 1), (1,1), (1, -1)):
                for i in range(d):
                    if x in range(self.width) and y in range(self.height):
                        yield self.cellsGrid[x][y]
                    x+=dx; y+=dy
            x+=1 
        
class Cell(MagicObject):
    """  """
    def __init__(self):
        MagicObject.__init__(self)
        self.entities = []
        self.objects = []

class Entity(MagicObject):
    """  """
    def __init__(self):
        MagicObject.__init__(self)
        self.quests = []
        self.inventory = []
        self.user = None

plurals = {"maps":Map, "entities":Entity, "cells":Cell, "objects":MagicObject,
       "types":ObjectType, "inventory":MagicObject, "quests":MagicObject}

