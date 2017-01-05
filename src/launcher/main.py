import gui
import socket
import hashlib
from const import *
import os
import glob

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
    global LINE_NUM
    global GUI
    msg = "{}\t[{}]\t{}".format(LINE_NUM,tag, msg)
    LINE_NUM += 1
    if GUI is not None:
        if GUI.running:
            GUI.log_msg(msg)
        else:
            exit(0)
    else:
        print(msg)

def run_client():
    done = False

    s = socket.socket()
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
        s.connect((socket.gethostbyname(host), SYNC_PORT))
    except:
        log_msg('ERROR', 'Connection failed')
        s.close()
        return
    log_msg('CONNECTION', 'Connection successful to ' + host + 'on port ' + str(port))

    log_msg('TRANSFER', 'Starting transfer protocol')

    while True:
        filename = s.recv(BUF)
        if filename == b'done':
            log_msg('TRANSFER', 'Transfer protocol ended. All files synchronized.')
            break

        log_msg('TRANSFER', 'Synchronizing file ' + filename)
        try:
            f = open(filename, 'rb')
            filemd5 = hashlib.md5(f.read()).hexdigest()
            f.close()
        except FileNotFoundError:
            log_msg('DEBUG', 'Missing file ' + filename)
            filemd5 = 0

        s.send(filemd5.encode('utf-8'))
        log_msg('DEBUG', 'Sending md5 ' + filemd5)
        ans = s.recv(BUF)

        if ans == 'ms':
            f = open(filename, 'wb')
            log_msg('DEBUG', 'Missing file or wrong version')
            l = s.recv(BUF)
            while (l):
                log_msg('DEBUG', 'Recieving file from server')
                f.write(l)
                l = s.recv(BUF)
            f.close()
        log_msg('TRANSFER', 'File ' + filename + 'synchronized')

    s.shutdown(socket.SHUT_WR)
    s.close()


def run_server(folder):

    filesToSend = glob.iglob(PATH + folder + '/*', recursive=True)
    log_msg('FILES', 'Folder ' + PATH + folder + ' is selected for synchronization')
    log_msg('FILES', 'Files to be synchronized :')

    for filename in filesToSend:
        log_msg('FILES', '\t '+ filename)

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

    forkedpids = []
    log_msg('CONNECTION', 'Waiting for clients')

    while True:
        csock, addr = psock.accept()
        log_msg('CONNECTION', 'Accepted connection from ' + str(addr))

        pid = os.fork()
        if pid == 0:
            log_msg('TRANSFER', 'Starting transfer protocol')

            for filename in filesToSend:
                csock.send(filename.encode('utf-8'))
                log_msg('TRANSFER', 'Processing file ' + filename)
                f = open(filename, 'rb')
                filemd5 = hashlib.md5(f.read()).hexdigest().encode('utf-8')
                log_msg('DEBUG', 'Expecting md5 : ' + filemd5)
                recvmd5 = csock.recv(BUF)
                log_msg('DEBUG', 'Got md5 : ' + recvmd5)
                if recvmd5 != filemd5:
                    log_msg('DEBUG', 'Missing file or wrong version')
                    csock.send(b'ms')
                    buf = f.read(BUF)
                    while (buf):
                        csock.send(buf)
                        buf = f.read(BUF)
                else:
                    log_msg('DEBUG', 'Correct file version')
                    csock.send(b'ok')
                f.close()
                log_msg('TRANSFER', 'File ' + filename + ' complete')

            log_msg('TRANSFER', 'Transfer protocol ended. All files synchronized.')
            csock.send(b'done')
            csock.close()
            exit(0)



if __name__ == "__main__":

    GUI = gui.MainUI()

    while True:
        if not GUI.running:
            break
        GUI.update()

    if GUI.server is None:
        exit(0)
    elif GUI.server:
        GUI = gui.ServerUI(run_server)
    else:
        GUI = gui.ClientUI(run_client)

    while True:
        if not GUI.running:
            break
        GUI.update()
