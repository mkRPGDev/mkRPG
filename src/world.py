from enum import IntEnum
from collections import defaultdict
from random import randint

from utils import readXml
from orders import Order, OrderType
import parsing.global_parsing as global_parsing
#l'autre solution est de tout mettre dans une fonction
# qui écrasera des classes bidon en global

verbose = True

named = {}
toResolve = []

world = None

def loadGame(path):
    """ Lit le game.xml et renvoie le monde chargé """
    global world
    parsed_data = global_parsing.game_parser(path+"game.xml")
    print(parsed_data)
    for data_list in parsed_data:
        if data_list != 'Actions' and  data_list != 'Interactions':
            typ = None
            if data_list.endswith("Type"):
                typ = data_list[:-4]
            for data in parsed_data[data_list]:
                eval(data_list)().load(data, typ)

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

    def __str__(self):
        return "Représentation de l'objet %s : %s" % (self.__class__, self.params)

    def load(self, data, typ=None):
        """ Charge l'objet depuis une structure Xml """
        if verbose: print("Data to load~: %s" % data)
        if typ != None and type(eval(typ)) == type:
            for key in data.keys():
                ObjectType(typ).load(data)

        for key in data.keys():
            if key == 'name':
                continue
            elif key == 'params':
                for sub_data in data[key].keys():
                    if type(data[key][sub_data])==dict and data[key][sub_data].get("id"):
                        toResolve.append((data[key][sub_data].get("id"), self.params, key))
                    else:
                        self.params[sub_data] = int(data[key][sub_data])
            elif key.lower() in self.__dict__ and\
                type(self.__dict__[key.lower()])==list:

                lowered = key.lower()
                data_collected = self.__dict__[lowered]
                class_type = plurals[lowered]
                for sub_data in data[key]:
                    if sub_data.get('id'):
                        toResolve.append((sub_data['id'], data_collected, len(data_collected)))
                        data_collected.append(None)
                    else:
                        data_collected.append(class_type().load(sub_data))
            else:
                self.params[key] = data[key]
        if 'name' in data.keys():
            named[data['name']] = self
#        for d in data.list:
#            n = d.name
#            if n=="Params": #peut gérer plusieurs def de params
#                for np, ap, _ in d.list:
#                    if "val" in ap:
#                        self.params[np] = int(ap["val"]) # on a supposé un int
#                    else:
#                        toResolve.append((ap["id"], self.params, np))
#            elif n==self.__class__.__name__+"Type": # ObjectType
#                pass
#            elif (n.lower() in self.__dict__ and 
#                 type(self.__dict__[n.lower()])==list):
#                li = self.__dict__[n.lower()]
#                C = plurals[n.lower()]
#                for dat in d.list:
#                    if "id" in dat.args:
#                        toResolve.append((dat.args["id"], li, len(li)))
#                        li.append(None)
#                    else:
#                        li.append(C().load(dat))
#            elif n.endswith("Type") and type(eval(n[:-4]))==type and "name" in d.args:
#                ObjectType(eval(n[:-4])).load(d)
#            elif type(eval(n))==type and "name" in d.args:
#                eval(n)().load(d)
        print("To resolve : %s" % toResolve)
        for nm, li, ln in toResolve: #TODO a optimiser
            if nm not in named: continue
            if verbose: print(nm, "-->", named[nm])
            print("li is %s" % li)
            li[ln] = named[nm]
            #assert nm in named, nm+" non résolu" FIXME
        print("Finished to load %s" % data)
        print("params of data %s" % self.params)
        return self

    def contextEval(self, value):
        """ Évalue une expression dans le contexte de l'objet pour les ordres """
        return eval(value)

    # TODO traitement d'ordres ?

# TODO à enlever ?
#class ServerObject(BaseObject): pass
#class ClientObject(BaseObject): pass

Object = BaseObject
#ServerObject if SERVER else ClientObject
# pour éviter la confusion avec object

class ObjectType(Object):
    """ Les types d'objets (au sens informatique) """
    def __init__(self, typ = Object):
        super().__init__()
        self.type = typ
    
    def create(self):
        """ Instanticiation d'un objet à partir du type """
        instance = self.type()
        instance.type = self
        for p,v in self.params.items():
            instance.params[p] = v
        return instance
    
class World(Object):
    def __init__(self):
        Object.__init__(self)
        self.maps = [] # une liste c'est mieux non ?
        self.entities = []
        self.objects = []
        
class Map(Object):
    def __init__(self):
        Object.__init__(self)
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
    
class Cell(Object):
    def __init__(self):
        Object.__init__(self)
        self.entities = []
        self.objects = []
        
class Entity(Object):
    def __init__(self):
        Object.__init__(self)
        self.quests = []
        self.inventory = []
        self.user = None
    
plurals = {"maps":Map, "entities":Entity, "cells":Cell, "objects":Object,
       "types":ObjectType, "inventory":Object, "quests":Object}

if __name__=="__main__":
    verbose = True
    w = World()
    w.load("../Test/") # désuet

