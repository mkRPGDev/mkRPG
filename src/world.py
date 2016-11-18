from enum import IntEnum
from collections import defaultdict
from random import randint

#from isserver import SERVER
from tools import readXml

from orders import Order, OrderType
#l'autre solution est de tout mettre dans une fonction
# qui écrasera des classes bidon en global

verbose = False

named = {}
toResolve = []

world = None

def loadGame(path):
    """ Lit le game.xml et renvoie le monde chargé """
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
    """ Tout objet du monde """
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
        """ Évalue une expression dans le contexte de l'objet pour les ordres """
        return eval(value)

    # TODO traitement d'ordres ?

#class ServerObject(BaseObject): pass
#class ClientObject(BaseObject): pass

MagicObject = BaseObject
# ServerObject if SERVER else ClientObject
# pour éviter la confusion avec object

class ObjectType(MagicObject):
    """ Les types d'objets (au sens informatique) """
    def __init__(self, typ = MagicObject):
        super().__init__()
        self.type = typ

    def create(self):
        """ Instanticiation d'un objet à partir du type """
        instance = self.type()
        instance.type = self
        for p,v in self.params.items():
            instance.params[p] = v
        return instance

    def __str__(self):
        return str(self.params)

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

    def fill(self):
        """ Complète les cases par défaut """
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
        return abs(source.x-dest.x) + abs(source.y-dest.y)
    
    def lov(self, source, dest):
        """ 
        Calcule si la vue est dégagée entre source et dest,
        ces derniers doivent avoir des paramètres x et y et
        les cellules de la carte un paramètre "visible"
        O(dist(source, dest)) environ
        TODO une méthode pour ce calcul sur de nombreuses cases 
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

class Cell(MagicObject):
    def __init__(self):
        MagicObject.__init__(self)
        self.entities = []
        self.objects = []

class Entity(MagicObject):
    def __init__(self):
        MagicObject.__init__(self)
        self.quests = []
        self.inventory = []
        self.user = None

plurals = {"maps":Map, "entities":Entity, "cells":Cell, "objects":MagicObject,
       "types":ObjectType, "inventory":MagicObject, "quests":MagicObject}

if __name__=="__main__":
    verbose = True
    w = World()
    w.load("../Test/") # désuet
