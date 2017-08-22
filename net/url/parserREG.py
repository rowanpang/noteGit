#!/usr/bin/env python 
import socket
import httplib 
import sys
import re
import time

f = open('./hostReg.txt','w')
for l in open('./url.txt').readlines():
    ret = re.findall('(\w+)://([a-zA-Z0-9.-]+):?(\d{1,4})?(/?\S+)?\s?$',l)[0]
    if not ret:
        print l.strip()
        sys.exit()
    print ret
    proto = ret[0]
    host = ret[1]
    port = int(ret[2]) if ret[2] else None 
    if port:
        f.write('%s://%s:%d\n' %(proto,host,port))
    else:
        f.write('%s://%s\n' %(proto,host))

f.close()
