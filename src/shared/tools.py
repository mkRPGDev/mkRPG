from collections import namedtuple
from time import process_time, time
from xml.parsers.expat import ParserCreate
from threading import Thread
from heapq import heappush, heappop
#from queue import Queue
import asyncio

verbose = False

Node = namedtuple("Node", "name args list")
Node.__doc__ = """ Xml node with the name of the markup,
               its embedded parameters and a list of sub-nodes """

#TODO mettre une grammaire sur le Xml
def readXml(path):
    """ Read a Xml file and returns imbricated Nodes """
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
    return curDir.list[0]

class Perf:
    """ Times a piece of code """
    def __init__(self):
        self.num = 0
        self.avg = 0
        self.min = 1000000
        self.max = 0

    def tic(self):
        """ To be run before the timed function """
        self.t = process_time()

    def toc(self):
        """ To be run after the timed function """
        dt = process_time()-self.t
        self.num += 1
        self.avg += (dt-self.avg)/self.num
        self.max = max(self.max, dt)
        self.min = min(self.min, dt)

    def show(self):
        """ Print stats """
        if self.num:
            print("Average time %es, minimum %es, maximum %es over %d executions."
                  % (self.avg, self.min, self.max, self.num))

class Timer:
    """ Trigger differed couroutine calls """
    def __init__(self, timeFunc = time):
        self.dt = 0.001
        self.step = 0
        self.heap = []
        self.pause = False
        self.count = 0 # pour avoir un ordre total
        self.time = timeFunc

    def add(self, time, func, args):
        """ Write the function func call with arguments args """
        self.count += 1
        heappush(self.heap, (time + self.step, self.count, (func, args)))

    async def run(self):
        """ Working task """
        while True:
            while self.pause: await asyncio.sleep(self.dt)
            begin = self.time()
            self.step += 1
            while self.heap and self.heap[0][0] == self.step:
                func, args = heappop(self.heap)[2]
                await func(*args)
            await asyncio.sleep(max(0, self.dt - self.time() + begin))


if __name__=="__main__":
    # tests
    #verbose = True
    #perff = Perf()
    #print(readXml("../Test/Snake/world.xml"))
    #perff.show()
    loop = asyncio.get_event_loop()
    t = Timer(loop.time)
    t.dt = 1
    t.add(1, print, [1])
    t.add(2, print, [2])
    t.add(1, print, [11])
    loop.run_until_complete(t.run())
