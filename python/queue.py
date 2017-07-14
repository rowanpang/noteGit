#!/usr/bin/python
#coding: utf-8

import Queue
import threading
import time

def consumer(q):
    while True:
        while True:
            i = q.get()
            if q.empty():
                break;
        print 'consumer get %s' %i
        time.sleep(3)

def producter(q):
    while True:
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

    time.sleep(100)
