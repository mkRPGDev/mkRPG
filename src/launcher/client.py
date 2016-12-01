import socket
import hashlib
from const import *

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
