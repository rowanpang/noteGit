#!/usr/bin/python
import os
import time
import subprocess
import socket
import select

def say_hello():
    print 'hello world,from fathre.py'

if __name__ == '__main__':
    say_hello()
    #p=subprocess.Popen(["/sbin/chkconfig","--list"],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE);
    #p.wait()
    #print p.stdout.read()

    fask=socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
    os.system("rm -rf ./sk_file")
    fask.bind("./sk_file")
    fask.listen(1)
    while 1:
        conn,addr = fask.accept()
        #select.select([fask],[],[])
        son_info=conn.recv(1024)
        print son_info
    fask.close()
    #os.system("./son.py")
    #print(time.localtime())
