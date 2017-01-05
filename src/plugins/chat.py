"""
This describes the chat plugin.
This is only the system that handles the chat and not the module
that displays the chat on the screen, since it is UI dependent (curses or pygame).
"""

from plugins.plugin import Plugin

class Chat(Plugin):
    """ Handles a minimalist chat plugin to be used in the game. """
    MSGID = b"chat"

    def __init__(self, e):
        super().__init__(e)
        self.msgs = ["Connected"]

    def serverMessage(self, msg):
        """ Broadcasts a message on whole clients. """
        self.broadcast(msg)

    def clientMessage(self, msg):
        """ Receives a message and appends it to the messages to be displayed.
        Then, it repaints the screen in order to update the view of the chat.
        """
        self.msgs.append(msg)
        if self.ui:
            self.ui.repaint()
