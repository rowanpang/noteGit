#!/usr/bin/python
#encoding utf-8

import threading
import time

lock = threading.Lock()

def Count(id):
    global num
    i = 0
    time.sleep(1)
    for i in range(1000):
        lock.acquire()
        num += 1
        lock.release()
    print "Thread id is : %s     The num is %s" % (id, str(num))
    time.sleep(2)

if __name__ == "__main__":
    num = 0
    A = threading.Thread(target = Count,args=('A',))
    B = threading.Thread(target = Count,args=('B',))

    A.start()
    B.start()

    A.join()
    B.join()
