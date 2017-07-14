#!/usr/bin/python
#encoding utf-8

import thread
import time

lock = thread.allocate_lock()

def Count(id):
    global num
    i = 0
    while True:
        lock.acquire()
        if i < 10:
            print "Thread id is : %s     The num is %s" % (id, str(num))
            i += 1
            num += 1
        lock.release()

if __name__ == "__main__":
    num = 0
    A = thread.start_new(Count,('A',))
    B = thread.start_new(Count,('B',))
    print (A)
