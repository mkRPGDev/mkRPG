import tkinter as tk
from tkinter.scrolledtext import ScrolledText

class MainUI:

    def __init__(self):
        self.root = tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.running = True

        tk.Button(self.root, text='Client', command=self.run_client, width=20).grid(row=1, column=1)
        tk.Button(self.root, text='Server', command=self.run_server, width=20).grid(row=1, column=2)

        self.root.update_idletasks()
        self.root.update()

    def on_closing(self):
        self.running = False
        self.root.destroy()

    def run_client(self):
        pass

    def run_server(self):
        pass

    def update(self):
        self.root.update_idletasks()
        self.root.update()

    def run(self):
        self.root.mainloop()
