from enum import IntEnum
from collections import defaultdict
from random import randint

from shared.tools import readXml
from shared.orders import Order, OrderType
import parsing.global_parsing as global_parsing

verbose = True

named = {}
toResolve = []

world = None

def loadGame(path):
    """ Lit le game.xml et renvoie le monde chargé """
    global world
    parsed_data = global_parsing.game_parser(path+"game.xml")
    for data_list in parsed_data:
        if data_list != 'Actions' and  data_list != 'Interactions':
            for data in parsed_data[data_list]:
                eval(data_list)().load(data, typ=data_list)

    world = named["world"]
    for m in world.maps: m.fill()
    print("Finished loading world. What lasts:\n%s" % toResolve)
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
        if typ != None and type(eval(typ)) == type and typ.endswith("Type"):
            for key in data.keys():
                ObjectType(typ[:-4]).load(data)
        elif typ != None and (type(eval(typ))) == type:
            eval(typ)().load(data)

        for key in data.keys():
            if key == 'name':
                continue
            elif key == 'params':
                for sub_data in data[key].keys():
                    if type(data[key][sub_data])== dict and data[key][sub_data].get("id"):
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
            print("data name is : %s" % data['name'])
            named[data['name']] = self
        for nm, li, ln in toResolve: #TODO a optimiser
            if nm not in named: continue
            if verbose: print(nm, "-->", named[nm])
            li[ln] = named[nm]
            #assert nm in named, nm+" non résolu" FIXME
        return self

    def contextEval(self, value):
        """ Évalue une expression dans le contexte de l'objet pour les ordres """
        return eval(value)

    # TODO traitement d'ordres ?

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

    def __str__(self):
        return str(self.params)

    
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
    
    def dist(self, source, dest):
        return abs(source.x-dest.x) + abs(source.y-dest.y)
    
    def lov(self, source, dest):
        """ 
        Calcule si la vue est dégagée entre source et dest,
        ces derniers doivent avoir des paramètres x et y et
        les cellules de la carte un paramètre "visible"
        O(dist(source, dest)) environ
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
        Assuming >x vy it turns as the trigo circle,
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
