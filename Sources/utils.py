from collections import namedtuple
import xml.parsers.expat
import os

#TODO mettre une grammaire sur tout ce petit monde
verbose = False

Node = namedtuple("Node", "name args list")

def readXml(path):
    """ lit un xml et renvoie des dictionnaires imbriqués """
    dirStack = [] #TODO stackiser
    curDir = Node(None,None,[])
    def start(name, attrs):
        nonlocal curDir
        if verbose: print('Start element:', name, attrs)
        curDir.list.append(Node(name, dict(attrs), []))
        dirStack.append(curDir)
        curDir = curDir.list[-1]
    def end(name):
        nonlocal curDir
        if verbose: print('End element:', name)
        assert(curDir.name == name)
        curDir = dirStack.pop()

    pars = xml.parsers.expat.ParserCreate()
    pars.StartElementHandler = start
    pars.EndElementHandler = end

    with open(path, 'rb') as file:
        pars.ParseFile(file)
    return curDir.list[0] #retrait des sur-emballages ça pollue

def loadWorld(path):
    world = World()
    world.load(readXml(path+"world.xml"))
    for m in os.listdir("maps"):
        ma = Map()
        ma.load(readXml(path+"maps/"+m))
        world.maps.add(ma)
    for m in os.listdir("entities"):
        ma = Entity()
        ma.load(readXml(path+"entities/"+m))
        world.entities.add(ma)
    return world

def fakeWorldServ():
    w = World()
    m = Map()
    w.maps.add(m)
    

if __name__=="__main__":
    # tests
    verbose = True
    print(readXml("../Test/world.xml"))
