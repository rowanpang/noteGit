#!/usr/bin/python

import os
import sys
import subprocess
from subprocess import Popen,PIPE

# ok
# p=Popen("ls")
# p=Popen(["ls", "-lia"])
#print type(p)

# syntax error
# p=Popen("ls -lia")

p=Popen(["ls", "-lia"],stdout=PIPE,stderr=PIPE)
p.wait()
subRet=p.communicate()
print type(subRet)
print subRet[0] 
# print subRet[1] 
print type(subRet[0])
for line in subRet[0].splitlines():
    print line

p=Popen(["ip", "link"],stdout=PIPE,stderr=PIPE)
p.wait()
subRet=p.communicate()
# print subRet[0]
master='wlp0s20u7u4'
for line in subRet[0].splitlines():
    # print line
    iterms = line.split(':')
    # print iterms
    if iterms[1].find(master) != -1:
        print iterms[1]
        print iterms[1].lstrip().split('@')[0]
