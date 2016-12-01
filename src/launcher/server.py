import socket
import hashlib
from const import *
import os
import glob

print('Fichiers à synchroniser :')
filesToSend = glob.iglob(PATH+'/*', recursive=True)
for filename in filesToSend:
    print('\t'+filename)

psock = socket.socket()
host = socket.gethostname()
psock.bind((host, SYNC_PORT))
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
