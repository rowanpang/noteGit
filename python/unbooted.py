#!/usr/bin/python

import binascii
import datetime
import os
import sys

def usage():
    print 'Usage: %s /dev/sdX' %(os.path.basename(sys.argv[0]))

if len(sys.argv) != 2:
    usage()
    sys.exit()

dev = sys.argv[1]
ovrSize = 0x1b8                             #440 bytes
print '-------ATTENTION dev:%s,ovrSize:%d-------' %(dev,ovrSize)
if raw_input('please confirm [y/N](N):') != 'y':
    sys.exit()

bkfile='/root/mbr-org-%s' %(datetime.datetime.now().strftime('%Y-%m-%d-%H:%M:%S'))
mbrOrg = open(dev).read(512) 
# print binascii.hexlify(mbrOrg)
print '----bkfile %s' %(bkfile)
open(bkfile,'w').write(mbrOrg)

mbrNew = '\00' * ovrSize + mbrOrg[ovrSize:]
# print binascii.hexlify(mbrNew)
open(dev,'w').write(mbrNew)
