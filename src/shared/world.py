from enum import IntEnum
from collections import defaultdict, OrderedDict
from random import randint # utilisé par eval

from shared.const import IDLEN
from shared.orders import Order, OrderType
from parsing import global_parsing

verbose = False

named = OrderedDict()
toResolve = []

world = None

def loadGame(parsed_data):
    """ Load game.xml and returns the loaded world """
    global world
    for data_list in parsed_data:
        if data_list not in ('Actions', 'Interactions', 'Images', 'Plugins'):
            for data in parsed_data[data_list]:
                ident = data.pop('ident')
                eval(data_list)(ident).load(data, typ=data_list)
    world = named['world']
    for obj in Object.ids.values():
        obj.initParams = dict(obj.params) # copie pour calculer la différence
    for m in world.maps:
        m.fill()
    return world

def retrieveWorld():
    orders = []
    for key, obj in Object.ids.items():
        # l'OrderedDict assure que les créations vont être faites dans le même ordre
        orders.extend(obj.getDifference())
    return orders

class Object:
    """ Any world object """
    ids = OrderedDict() # liste si sans deletion
    
    numid = [(1<<8*IDLEN)] 
    # XXX liste pour avoir une référence modifiable depuis tous les objets

    def __init__(self, identifier=None):
        if identifier is None:
            Object.numid[0] -= 1
            identifier = Object.numid[0]
        assert identifier != 0 # l'id 0 est réservé pour les plugins
        assert identifier not in Object.ids # un id est en double
        Object.ids[identifier] = self
        self.params = {} # Ne pas déplacer =)
        self.ident = identifier
        self.creator = None # indique si l'objet a été créé avec un type
        self.conditions = defaultdict(lambda:defaultdict(list)) #TODO à déplacer

    def __getattr__(self, attr):
        if attr in self.params:
            return self.params[attr]
        raise AttributeError(repr(self)+" has no "+repr(attr))

    def __setattr__(self, attr, val):
        if attr is "params" or attr not in self.params:
            object.__setattr__(self, attr, val)
        else:
            self.params[attr] = val

    def load(self, data, typ=None):
        """ Charge l'objet depuis une structure Xml """

        _type = data.get("type")
        if _type:
            typ = _type
        if typ and typ.endswith("Type") and type(eval(typ[:-4])) == type:
            data.pop('type', None)
            if verbose: print(data)
            del self.ids[self.ident]
            ObjectType(self.ident, eval(typ[:-4])).load(data)
        else:
            for key in data.keys():
                if key == 'params':
                    for sub_data in data[key].keys():
                        if type(data[key][sub_data])== dict and data[key][sub_data].get("id"):
                            toResolve.append((data[key][sub_data].get("id"), self.params, sub_data))
                        else:
                            self.params[sub_data] = int(data[key][sub_data])
                elif key == self.__class__.__name__+"Type":
                    pass
                elif key.lower() in self.__dict__ and\
                    type(self.__dict__[key.lower()]) == list:

                    lowered = key.lower()
                    data_collected = self.__dict__[lowered]
                    class_type = plurals[lowered]
                    for sub_data in data[key]:
                        if sub_data.get('id'):
                            toResolve.append((sub_data['id'], data_collected, len(data_collected)))
                            data_collected.append(None)
                        else:
                            ident = sub_data.pop("ident")
                            data_collected.append(class_type(ident).load(sub_data))
                elif typ and type(eval(typ))==type and 'name' in data[key]:
                    ident = data[key]['ident']
                    eval(typ)(ident).load(data[key], typ)

            if 'name' in list(data):
                named[data['name']] = self
        for nm, li, ln in toResolve: #TODO a optimiser
            if nm not in named: continue
            if verbose: print(nm, "->", named[nm])
            li[ln] = named[nm]
            #assert nm in named, nm+" non résolu" FIXME
        return self

    def contextEval(self, value):
        """ Eval an expression in the context of the object for orders """
        return eval(value)

    def getDifference(self):
        """ Generates a list of order that permits the object retrieval """
        orders = []
        if self.creator:
            order = Order()
            order.setType(OrderType.Create)
            order.event = ""
            order.base = str(self.creator.ident)
            order.init = ""
            orders.append((self.ident, order))
        for key in self.params:
            if self.params[key] != self.initParams[key]:
                order = Order()
                if isinstance(self.params[key], int):
                    order.setType(OrderType.Set)
                    order.value = str(self.params[key])
                else:
                    order.setType(OrderType.Setobj)
                    order.value = "world.ids["+str(self.params[key].ident)+"]"
                order.target = "world.ids["+str(self.ident)+"]"
                order.param = key
                orders.append((self.ident, order))
        return orders
                                

class ObjectType(Object):
    """ Les types d'objets (au sens informatique) """
    def __init__(self, ident=None, typ = Object):
        super().__init__(ident)
        self.type = typ

    def __str__(self):
        return (str(self.params) + "\ntype = %s" % self.type)

    def create(self, ident=None):
        """ Instanticiation d'un objet à partir du type """
        instance = self.type(ident)
        instance.creator = self
        instance.type = self
        for p,v in self.params.items():
            instance.params[p] = v
        instance.initParams = dict(instance.params)
        return instance


class World(Object):
    def __init__(self, ident=None):
        Object.__init__(self, ident)
        self.maps = [] # une liste c'est mieux non ?
        self.entities = []
        self.objects = []

class Map(Object):
    """ Orthonormed map with associated alogrithms """
    def __init__(self, ident=None):
        Object.__init__(self, ident)
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
                    cell.creator = None
                    # bien que créés à la volée, la création a lieu des deux cotés
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

class Cell(Object):
    def __init__(self, ident=None):
        Object.__init__(self, ident)
        self.entities = []
        self.objects = []

    def __str__(self):
        return(str(self.params))


class Entity(Object):
    def __init__(self, ident=None):
        Object.__init__(self, ident)
        self.quests = []
        self.inventory = []
        self.user = None

    def __str__(self):
        return str(self.params)


plurals = {"maps":Map, "entities":Entity, "cells":Cell, "objects":Object,
       "types":ObjectType, "inventory":Object, "quests":Object}
