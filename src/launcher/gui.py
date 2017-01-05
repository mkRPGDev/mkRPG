# -*- coding: utf-8 -*-

"""
    GUI module for file synchronizer
"""

import tkinter as tk
from tkinter.scrolledtext import ScrolledText
from urllib.request import urlopen
import socket

class MainUI:
    """ Main GUI with Client/Server buttons """

    def __init__(self):
        self.server = None

        self.root = tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.running = True

        tk.Button(self.root, text='Client', command=self.run_client, width=20).grid(row=1, column=1)
        tk.Button(self.root, text='Server', command=self.run_server, width=20).grid(row=1, column=2)

        self.root.update_idletasks()
        self.root.update()

        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()
        window_height = self.root.winfo_height()
        window_width = self.root.winfo_width()

        self.root.geometry('%dx%d+%d+%d' % (window_width, window_height,
                                            screen_width/2 - window_width/2,
                                            screen_height/2 - window_height/2))

        self.root.update()

    def on_closing(self):
        """ Closes the window """
        self.running = False
        self.root.destroy()

    def run_client(self):
        """ To be called on client selection """
        self.server = False
        self.on_closing()

    def run_server(self):
        """ To be called on server selection """
        self.server = True
        self.on_closing()

    def update(self):
        self.root.update_idletasks()
        self.root.update()

    def run(self):
        self.root.mainloop()

class ClientUI:
    """ Client window class """

    def __init__(self, client):
        """
            Client window class constructor

            @param client the client core function to be called on running
        """
        self.root = tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.running = True

        self.log = ""
        self.client = client

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

        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()
        window_height = self.root.winfo_height()
        window_width = self.root.winfo_width()

        self.root.geometry('%dx%d+%d+%d' % (window_width, window_height,
                                            screen_width/2 - window_width/2,
                                            screen_height/2 - window_height/2))

        self.root.update()

    def on_closing(self):
        """ Closes the window """
        self.running = False
        self.root.destroy()

    def run_client(self):
        """ Runs the client core and toogles the led """
        self.led.config(bg='green')
        try:
            self.client()
        except:
            self.led.config(background='red')
        self.led.config(background='red')

    def log_msg(self, msg, end="\n"):
        """ Logs a message into the prompt area """
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
    """ Server window class """


    def __init__(self, run_server, world_list):
        """
            Server window class constructor

            @param run_server the server core function to be called on running
            @param world_list the world list to be potentially synchronized
        """
        self.root = tk.Tk()
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.running = True

        self.log = ""
        self.server = run_server

        self.S = tk.Scrollbar(self.root)
        self.prompt = tk.Text(self.root, width=70)

        self.led = tk.Label(self.root, background='red')
        tk.Label(self.root, text ='Global IP :').grid(row=1, column=0)
        tk.Label(self.root, text =urlopen('http://ip.42.pl/raw').read().decode("utf-8")).grid(row=1, column=2)
        tk.Label(self.root, text ='Local IP :').grid(row=2, column=0)
        tk.Label(self.root, text =socket.gethostbyname(socket.gethostname())).grid(row=2, column=2)
        tk.Label(self.root, text ='Port : ').grid(row=3, column=0)
        self.port = tk.Entry(self.root)

        self.available_worlds = tuple([''] + world_list)
        self.world = tk.StringVar()
        tk.OptionMenu(self.root, self.world, *self.available_worlds).grid(row=1, column=3, sticky='E')

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

        screen_width = self.root.winfo_screenwidth()
        screen_height = self.root.winfo_screenheight()
        window_height = self.root.winfo_height()
        window_width = self.root.winfo_width()

        self.root.geometry('%dx%d+%d+%d' % (window_width, window_height,
                                            screen_width/2 - window_width/2,
                                            screen_height/2 - window_height/2))

        self.root.update()

    def on_closing(self):
        """ Closes the window """
        self.running = False
        self.root.destroy()

    def run_server(self):
        """ Runs the client core and toogles the led """
        self.led.config(background='green')
        try:
            self.server(self.world.get())
        except:
            pass
        self.led.config(background='red')


    def log_msg(self, msg, end="\n"):
        """ Logs a message into the prompt area """
        self.log += msg+end
        self.prompt.delete('1.0', tk.END)
        self.prompt.insert(tk.END, self.log)
        self.prompt.yview(tk.END)

    def update(self):
        self.root.update_idletasks()
        self.root.update()

    def run(self):
        self.root.mainloop()
