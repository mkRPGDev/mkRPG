from plugins.plugin import Plugin
from const import IDLEN

class Chat(Plugin):
    MSGID = b"chat"

    def __init__(self, e):
        super().__init__(e)
        self.msgs = ["Connecté"]

    def serverMessage(self, msg):
        self.broadcast(msg)

    def clientMessage(self, msg):
        self.msgs.append(msg)
        if self.ui: self.ui.repaint()

