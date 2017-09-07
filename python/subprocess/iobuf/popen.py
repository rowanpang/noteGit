#!/usr/bin/python
import os
import subprocess
import time
if __name__ == '__main__':
    p=subprocess.Popen("./app",stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    print p.stdout.readline()
    p.stdin.write('hello\n')
    # time.sleep(2)
    p.stdin.write('world\n')
    time.sleep(5)
    print p.stdout.read()
