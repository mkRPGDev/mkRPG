from collections import namedtuple
from time import process_time
from xml.parsers.expat import ParserCreate

verbose = False

Node = namedtuple("Node", "name args list")

#TODO mettre une grammaire sur le Xml
def readXml(path):
    """ Lit un fichier Xml et renvoie des Nodes imbriqués """
    dirStack = [] #TODO stackiser
    curDir = Node(None,None,[])
    def start(name, attrs):
        nonlocal curDir
        #perff.tic()
        if verbose: print('Start element:', name, attrs)
        curDir.list.append(Node(name, dict(attrs), []))
        dirStack.append(curDir)
        curDir = curDir.list[-1]
        #perff.toc()

    def end(name):
        nonlocal curDir
        if verbose: print('End element:', name)
        assert(curDir.name == name)
        curDir = dirStack.pop()

    pars = ParserCreate()
    pars.StartElementHandler = start
    pars.EndElementHandler = end

    with open(path, 'rb') as file:
        pars.ParseFile(file)
    return curDir.list[0] #retrait des sur-emballages

class Perf:
    """ Calcule les performances d'un morceau de code """
    def __init__(self):
        self.num = 0
        self.avg = 0
        self.min = 1000000
        self.max = 0

    def tic(self):
        """ À lancer avant la fonction """
        self.t = process_time()

    def toc(self):
        """ À lancer après la fonction """
        dt = process_time()-self.t
        self.num += 1
        self.avg += (dt-self.avg)/self.num
        self.max = max(self.max, dt)
        self.min = min(self.min, dt)

    def show(self):
        """ Affiche le rapport """
        if self.num:
            print("Temps moyen %es, minimum %es, maximum %es sur %d éxécutions." % (self.avg, self.min, self.max, self.num))

if __name__=="__main__":
    # tests
    verbose = True
    perff = Perf()
    print(readXml("../Test/Snake/world.xml"))
    perff.show()
