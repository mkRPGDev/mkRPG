# -*- coding: utf-8 -*-

"""
    File synchronizer

    @todo Multithread server and GUI for smooth GUI
    @todo Add folder creation to protocol
"""

import socket
import hashlib
import os
import glob
from argparse import ArgumentParser
import gui

HOST = "localhost"
PATH = "./test/"
SYNC_PORT = 12345
BUF = 1024
SYNC_MAX_SERVER_QUEUE = 5

LINE_NUM = 1
GUI = None

def log_msg(tag, msg):
    """
        Prints a message either to stdin or to the GUI

        @param tag a tag for message classification, for instance 'DEBUG'
        @param msg the value of the message to be printed
    """
    if not ARGS.verbose and tag == 'DEBUG':
        return
    global LINE_NUM
    global GUI
    msg = "{}\t[{}]\t{}".format(LINE_NUM, tag, msg)
    LINE_NUM += 1
    if GUI is not None:
        if GUI.running:
            GUI.log_msg(msg)
        else:
            exit(0)
    else:
        print(msg)

def run_client():
    """ Runs the client core """

    sock = socket.socket()
    if GUI is None:
        host = input("Enter host IP : ")
        port = SYNC_PORT
    else:
        try:
            host = GUI.ip.get()
            port = int(GUI.port.get())
        except:
            log_msg('ERROR', 'Wrong input')
            return

    log_msg('CONNECTION', 'Connection attempt to ' + host + ' on port ' + str(port))
    try:
        sock.connect((socket.gethostbyname(host), SYNC_PORT))
    except:
        log_msg('ERROR', 'Connection failed')
        sock.close()
        return
    log_msg('CONNECTION', 'Connection successful to ' + host + 'on port ' + str(port))

    log_msg('TRANSFER', 'Starting transfer protocol')

    while True:
        filename = sock.recv(BUF).decode('utf-8')
        if filename == 'done':
            log_msg('TRANSFER', 'Transfer protocol ended. All files synchronized.')
            break

        log_msg('TRANSFER', 'Synchronizing file ' + filename)
        try:
            file_descr = open(filename, 'rb')
            filemd5 = hashlib.md5(file_descr.read()).hexdigest()
            file_descr.close()
        except FileNotFoundError:
            log_msg('DEBUG', 'Missing file ' + filename)
            filemd5 = 0

        sock.send(filemd5.encode('utf-8'))
        log_msg('DEBUG', 'Sending md5 ' + filemd5)
        ans = sock.recv(BUF)

        if ans == 'ms':
            file_descr = open(filename, 'wb')
            log_msg('DEBUG', 'Missing file or wrong version')
            buf_data = sock.recv(BUF)
            while buf_data:
                log_msg('DEBUG', 'Recieving file from server')
                file_descr.write(buf_data)
                buf_data = sock.recv(BUF)
            file_descr.close()
        log_msg('TRANSFER', 'File ' + filename + 'synchronized')

    log_msg('CONNECTION', 'Closing connection')
    sock.shutdown(socket.SHUT_WR)
    sock.close()


def run_server(folder):
    """
        Runs the server core

        @param folder the PATH subfolder to be synchronized
    """

    if GUI is None:
        folder = input('Enter world name : ')

    files_to_send = glob.iglob(PATH + folder + '/*', recursive=True)
    log_msg('FILES', 'Folder ' + PATH + folder + ' is selected for synchronization')
    log_msg('FILES', 'Files to be synchronized :')

    for filename in files_to_send:
        log_msg('FILES', '\t '+ filename)
    files_to_send = glob.iglob(PATH + folder + '/*', recursive=True)

    try:
        port = SYNC_PORT if GUI is None else int(GUI.port.get())
    except:
        log_msg('ERROR', 'Wrong input')
        return

    log_msg('CONNECTION', 'Establishing server on port ' + str(port))
    try:
        psock = socket.socket()
        psock.bind(("", port))
        psock.listen(SYNC_MAX_SERVER_QUEUE)
    except:
        log_msg('ERROR', 'Socket error : unable to establish server')
        return

    log_msg('CONNECTION', 'Waiting for clients')

    while True:
        csock, addr = psock.accept()
        log_msg('CONNECTION', 'Accepted connection from ' + str(addr[0]))

        pid = os.fork()

        if pid == 0:
            log_msg('TRANSFER', 'Starting transfer protocol')
            for filename in files_to_send:
                csock.send(filename.encode('utf-8'))
                log_msg('TRANSFER', 'Processing file ' + filename)
                file_descr = open(filename, 'rb')
                filemd5 = hashlib.md5(file_descr.read()).hexdigest().encode('utf-8')
                log_msg('DEBUG', 'Expecting md5 : ' + filemd5.decode('utf-8'))
                recvmd5 = csock.recv(BUF)
                log_msg('DEBUG', 'Got md5 : ' + recvmd5.decode('utf-8'))
                if recvmd5 != filemd5:
                    log_msg('DEBUG', 'Missing file or wrong version')
                    csock.send(b'ms')
                    buf = file_descr.read(BUF)
                    while buf:
                        csock.send(buf)
                        buf = file_descr.read(BUF)
                else:
                    log_msg('DEBUG', 'Correct file version')
                    csock.send(b'ok')
                file_descr.close()
                log_msg('TRANSFER', 'File ' + filename + ' complete')

            log_msg('TRANSFER', 'Transfer protocol ended. All files synchronized.')
            csock.send(b'done')
            csock.close()
            exit(0)

PARSER = ArgumentParser(description="File synchronization program.")
PARSER.add_argument("-ns", "--nouiserver", action="store_true",
                    help="Launches terminal server without GUI.")
PARSER.add_argument("-v", "--verbose", action="store_true",
                    help="Activate verbose mode for file transfers.")
PARSER.add_argument("-nc", "--nouiclient", action="store_true",
                    help="Launches terminal client without GUI.")

if __name__ == "__main__":

    ARGS = PARSER.parse_args()

    if ARGS.nouiserver:
        run_server(None)
    elif ARGS.nouiclient:
        run_client()
    else:
        GUI = gui.MainUI()

        while True:
            if not GUI.running:
                break
            GUI.update()

        if GUI.server is None:
            exit(0)
        elif GUI.server:
            WORLD_LIST = [d for d in os.listdir(PATH)
                          if os.path.isdir(os.path.join(PATH, d))]
            GUI = gui.ServerUI(run_server, WORLD_LIST)
        else:
            GUI = gui.ClientUI(run_client)

        while True:
            if not GUI.running:
                break
            GUI.update()
