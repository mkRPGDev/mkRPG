from unittest import TestCase, main

class TestStringMethods(TestCase):

    def test_upper(self):
        self.assertEqual('foo'.upper(), 'FOO')

    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

class TestAction:
    def test_(self):
        self.assertEqual()

class TestClient:
    def test_(self):
        self.assertEqual()

class TestCurses:
    def test_(self):
        self.assertEqual()

class Test_registerInteractions:
    def test_(self):
        self.assertEqual()

class TestInterface:
    def test_(self):
        self.assertEqual()

class TestNetworkClient:
    def test_(self):
        self.assertEqual()

class TestNetworkServer:
    def test_(self):
        self.assertEqual()

class TestServerConnection:
    def test_(self):
        self.assertEqual()

class TestOrder:
    def test_(self):
        self.assertEqual()
    o = Order()
    o.type = OrderType.Set
    o.args = ["me", "myself", "I"]
    b = o.toBytes()
    oo = Order()
    oo.fromBytes(b)
    print(b)
    print(oo.__dict__==o.__dict__)
    
class TestOrderDispatcher:
    def test_(self):
        self.assertEqual()

class TestreadXml:
    def test_(self):
        self.assertEqual()

class TestPerf:
    def test_(self):
        self.assertEqual()

class TestTimer:
    def test_(self):
        self.assertEqual()

#class TestloadGame
#World...

if __name__ == '__main__':
    main()
