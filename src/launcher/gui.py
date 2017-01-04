import tkinter as tk
from tkinter.scrolledtext import ScrolledText
from urllib.request import urlopen
import socket


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

class ClientUI:

    def __init__(self):
        self.root = tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.running = True

        self.log = ""

        self.S = tk.Scrollbar(self.root)
        self.prompt = tk.Text(self.root, width=70)

        self.led = tk.Label(self.root, background='red')
        tk.Label(self.root, text ='IP :').grid(row=1, column=0)
        tk.Label(self.root, text ='Port :').grid(row=2, column=0)
        self.ip = tk.Entry(self.root)
        self.port = tk.Entry(self.root)

        tk.Button(self.root, text='Run', command=self.run_client, width=20).grid(row=1, column=3, sticky='E')
        tk.Button(self.root, text='Quit', command=self.on_closing, width=20).grid(row=2, column=3, sticky='E')

        self.prompt.grid(row=0, column=0, columnspan=4)
        self.S.grid(column=4, row=0, sticky='NS')
        self.ip.grid(row=1, column=2)
        self.port.grid(row=2, column=2)
        self.led.grid(row=1, column=4, rowspan=2, sticky='NSEW')

        self.S.config(command=self.prompt.yview)
        self.prompt.config(yscrollcommand=self.S.set)

        self.prompt.insert(tk.END, self.log)
        self.prompt.yview(tk.END)


        self.root.update_idletasks()
        self.root.update()

    def on_closing(self):
        self.running = False
        self.root.destroy()

    def run_client(self):
        pass

    def log_msg(self, msg, end="\n"):
        self.log += msg+end
        self.prompt.delete('1.0', tk.END)
        self.prompt.insert(tk.END, self.log)
        self.prompt.yview(tk.END)

    def update(self):
        self.root.update_idletasks()
        self.root.update()

    def run(self):
        self.root.mainloop()

class ServerUI:

    def __init__(self):
        self.root = tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.running = True

        self.log = ""

        self.S = tk.Scrollbar(self.root)
        self.prompt = tk.Text(self.root, width=70)

        self.led = tk.Label(self.root, background='red')
        tk.Label(self.root, text ='Global IP :').grid(row=1, column=0)
        tk.Label(self.root, text =urlopen('http://ip.42.pl/raw').read().decode("utf-8")).grid(row=1, column=2)
        tk.Label(self.root, text ='Local IP :').grid(row=2, column=0)
        tk.Label(self.root, text =socket.gethostbyname(socket.gethostname())).grid(row=2, column=2)
        tk.Label(self.root, text ='Port : ').grid(row=3, column=0)
        self.port = tk.Entry(self.root)

        available_worlds = ("world1", "world2")
        self.world = tk.StringVar()
        tk.OptionMenu(self.root, self.world, *available_worlds).grid(row=1, column=3, sticky='E')

        tk.Button(self.root, text='Run', command=self.run_server, width=20).grid(row=2, column=3, sticky='E')
        tk.Button(self.root, text='Quit', command=self.on_closing, width=20).grid(row=3, column=3, sticky='E')

        self.prompt.grid(row=0, column=0, columnspan=4)
        self.S.grid(column=4, row=0, sticky='NS')
        self.port.grid(row=3, column=2)
        self.led.grid(row=1, column=4, rowspan=3, sticky='NSEW')

        self.S.config(command=self.prompt.yview)
        self.prompt.config(yscrollcommand=self.S.set)

        self.prompt.insert(tk.END, self.log)
        self.prompt.yview(tk.END)


        self.root.update_idletasks()
        self.root.update()

    def on_closing(self):
        self.running = False
        self.root.destroy()

    def run_server(self):
        pass

    def log_msg(self, msg, end="\n"):
        self.log += msg+end
        self.prompt.delete('1.0', tk.END)
        self.prompt.insert(tk.END, self.log)
        self.prompt.yview(tk.END)

    def update(self):
        self.root.update_idletasks()
        self.root.update()

    def run(self):
        self.root.mainloop()
