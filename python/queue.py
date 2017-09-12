#!/usr/bin/python
#coding: utf-8

import Queue
import threading
import time

def consumer(q):
    time.sleep(1)
    while True:
        i = q.get()
        print 'consumer get %s' %i
        time.sleep(1)

def producter(q):
    i = 0
    while True:
        q.put(i)
        if i > 1000:
            break;
        i += 1
        time.sleep(1)

if __name__ == "__main__":
    q = Queue.Queue()

    g = threading.Thread(target = consumer,args=(q,))
    p = threading.Thread(target = producter,args=(q,))

    g.setDaemon(True)
    p.setDaemon(True)
    g.start()
    p.start()

    time.sleep(10000)
