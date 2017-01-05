# How to write a plugin ?
## What is a mkrpg plugin ?
A plugin is Python code that can be executed on both server and client side, adorning the game engine provided with new features (like displays). A simple API is detailed in this README to make plugins writable with little knowledge of the game implementation.

## Files
A plugin can be divided up to three parts: the logical one, the curses interface and the pygame interface. Those parts are related by a single name that will be referred as the name of your plugin. The next sessions will detail how to specify that three parts within files. All those files needs to be in the same directory, either in plugins/ or in a subdirectory of the latter. Keep in mind that any part can be omitted be simply omitting the file and that your allowed to import whatever you want.

## The logical part
The logical part has to be in _pluginname_.py and to contain a class named `Pluginname` that inherits `plugins.plugin.Plugin`.

For example : plugins/myplugin.py

    from plugins.plugin import Plugin
    class Myplugin(Plugin):
        pass
is a valid trivial plugin.

In order to use network functionalities you need to specify a Python's bytes that will be used to identify your message by adding a field `MSGID` to your class. Once this is done, you can re-implement several callbacks methods to make your plugin useful.

* `serverMessage(bytes message)` is called when the server receive a message for your plugin
* `clientMessage(bytes message)` is called when the client receive a message for your plugin

Within your plugin, you can also send messages by using `self.send(bytes message)` in code executed on the client and `self.broadcast(bytes message)` in the server.

Hence creating the logic of a simple chat that spawn every message is as simple as

    from plugins.plugin import Plugin

    class Chat(Plugin):
        MSGID = b"chat"

        def __init__(self, e):
            super().__init__(e)
            self.msgs = ["Connected"]

        def serverMessage(self, msg):
            self.broadcast(msg)

        def clientMessage(self, msg):
            self.msgs.append(msg)

To display your messages, to need to add a user-interface, and if you do so, `self.ui` will point on it, instead of `None`. Therefore add 

    if self.ui: self.ui.repaint()
    
to the last method and the logical part is over !

## Curses interface
The curses interface has to be in *pluginname*curses.py and to contain a class named `PluginnameView` that inherits `plugins.plugincurses.CursesPlugin`.

You can fill the fields `NAME` and `̀HELP` with bytes to have these texts displayed by the menu plugin.

### Placement
Choosing where your plugin will appear is complicated since clients are resizable.
To do so, fill in the fields `X`, `Y` with the position of the top-left corner of your plugin. You may put negative values to place it according to the right or the bottom edge. Then fill `MINW` and `MINH` with the size you need or put a number greater than the display to get the maximum space available.

### Drawing
Once placement is done, `̀self.win` is providing you a curses window where your interface will be drawn within a reimplementation of the `draw` method. The actual size of the window is provided as `self.width` and `self.height`. If you wrote a logical part to deal with network, it is available as `self.logic`.

The drawing itself will be done using methods of `self.win`. Erasing the window is up to you. On the contrary, please do not refresh your window, the client will proceed at the right moment.

### Handeling events
When an key is send to curses and is not caught by the client, the `handleKey` method of your view will be called with the keycode. After treatments, your method must return `True` if the key was treated and `False` otherwise. 

### An example
See plugins/chatcurses.py for a quite complete example.

## Pygame interface
Most of what said for curses is valid for pygame except the following points:

* You need to inherit `plugins.pluginpygame.PygamePlugin` and your class be named `PluginnameSprite` in a *pluginname*pyagme.py file.
* Placement is the same but you obtain `self.rect` and `self.image` that are used to draw your plugin which is a pygame Sprite.
* Mouse buttons are appearing in the `handleKey` method since they do not overlap with keyboard numbers. You can know where the mouse is by using the pygame `mouse.get_pos` function.

## Loading
While your plugin is ready for test, add an entry is the `game.xml` of your targetted game with your pluginname like :

    <Plugins>
       <Plugin>myplugin</Plugin>
    </Plugins>

Note that the order in which plugins are requested is important since the first plugins listed will have priority on placement and key handing.

## Advanced features
* The logical part grants you an `engine` attribute that refers to the `Client` or the `Server` class depending on what is running. You can tinker with everything from here.
* By taking the `MSGID` of another plugin and taking priority on it in loading, you can write plugins that changes the behavior of other plugins. The same can be done with keys that you faint to have ignored...

__Have fun with plugins now !__
