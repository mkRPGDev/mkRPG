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
        if self.ui:
            self.ui.repaint()
