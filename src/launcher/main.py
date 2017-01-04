import gui
import socket
import hashlib
from const import *
import os
import glob

def run_client():
    done = False

    s = socket.socket()
    host = input("Enter host IP : ")
    s.connect((socket.gethostbyname(host), SYNC_PORT))
    print('Connected')

    while True:
        filename = s.recv(BUF)
        if filename == b'done':
            break
        print('Server requested file ', filename)
        f = open(filename, 'rb')
        filemd5 = hashlib.md5(f.read()).hexdigest()
        s.send(filemd5.encode('utf-8'))
        print('\tFile md5 ', filemd5)
        ans = s.recv(BUF)
        if ans == 'ms':
            print('\tMissing or wrong version.')
            l = s.recv(BUF)
            while (l):
                print('\tRecieving file from server...')
                f.write(l)
                l = s.recv(BUF)
        print('Complete.')
        f.close()

    s.shutdown(socket.SHUT_WR)
    s.close()

def run_server():
    print('Fichiers à synchroniser :')
    filesToSend = glob.iglob(PATH+'/*', recursive=True)
    for filename in filesToSend:
        print('\t'+filename)

    psock = socket.socket()
    psock.bind(("", SYNC_PORT))
    psock.listen(SYNC_MAX_SERVER_QUEUE)

    forkedpids = []
    print('Waiting for clients.')

    while True:
        csock, addr = psock.accept()
        pid = os.fork()
        if pid == 0:
            for filename in filesToSend:
                csock.send(filename.encode('utf-8'))
                print('Requesting file ', filename)
                f = open(filename, 'rb')
                filemd5 = hashlib.md5(f.read()).hexdigest().encode('utf-8')
                print('\tExpecting md5 ', filemd5)
                recvmd5 = csock.recv(BUF)
                print('\tGot ', recvmd5)
                if recvmd5 != filemd5:
                    print('\tMissing file, sending...')
                    csock.send(b'ms')
                    buf = f.read(BUF)
                    while (buf):
                        buf = f.read(BUF)
                        csock.send(buf)
                else:
                    print('\tAlready have file.')
                    csock.send(b'ok')
                f.close()
                print('Complete.')
            csock.send(b'done')
            csock.close()
            exit(0)



if __name__ == "__main__":

    ui = gui.MainUI()

    while True:
        if not ui.running:
            break
        ui.update()

    if ui.server is None:
        exit(0)
    elif ui.server:
        ui = gui.ServerUI()
    else:
        ui = gui.ClientUI()

    while True:
        if not ui.running:
            break
        ui.update()
