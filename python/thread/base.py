#!/usr/bin/python
#coding=utf-8
import thread
import threading
from time import ctime,sleep


def music(func):
    for i in range(2):
        print "I was listening to %s. %s" %(func,ctime())
        sleep(100)

def move(func):
    for i in range(2):
        print "I was at the %s! %s" %(func,ctime())
        sleep(500)

threads = []
t1 = threading.Thread(target=music,args=(u'爱情买卖',))
threads.append(t1)

t2 = threading.Thread(target=move,args=(u'阿凡达',))
threads.append(t2)

if __name__ == '__main__':
    for t in threads:
        # t.setDaemon(True)
        t.start()

    for t in threads:
        while t.isAlive():
            t.join(1)
        # t.join(1)             this will block the SIGINT,so not use it.

    # while True:
        # sleep(50)

    print "all over %s" %ctime()
