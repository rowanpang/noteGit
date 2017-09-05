#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import os.path
import re
from subprocess import Popen,PIPE

rootdir =  '/home/pangwz/Downloads'                                 
rootdir =  '/sys/class/block'
gb = 1024*1024*1024
mb = 1024*1024
kb = 1024
lcdromid = os.path.join(__file__,'cdrom_id')
# print lcdromid

# for parent,dirnames,filenames in os.walk(rootdir):  
    # for dirname in  dirnames:                      
        # print "parent is:" + parent
        # print  "dirname is" + dirname
    # for filename in filenames:                    
        # print "parent is:" + parent
        # print "filename is:" + filename
        # print "the full name of the file is:" + os.path.join(parent,filename) 
for f in os.listdir(rootdir):
    print type(f),'   ',
    print '%4s is dir: %s' %((f), os.path.isdir('%s/%s' %(rootdir,f)))
    dd = rootdir + '/' + f + '/'
    if not re.match('^sd[a-z]+$|sr[0-9]+$',f):
        continue
    if open(dd + 'removable').read(1) == '0':
        continue
    print dd

    byts = float(open(dd + 'size').read().strip()) * 512
    if byts >= gb:        #G
        size = '%.2fG' %(byts/gb)
    elif byts >= mb:
        size = '%.2fM' %(byts/mb)
    elif byts >= kb:
        size = '%.2fK' %(byts/gb)
    else:
        size = '%s' %(byts)
    print size

    if f.startswith('sd'):
        devnode= '/dev/%s[0-9]+' %(f)
        dtype = 'disk'
        if not os.path.islink(dd[:-1]):
            pass
        if re.findall('usb[0-9]+\/',os.readlink(dd[:-1])):
            dtype = 'udisk'
        
        dst = 'inserted'
        for l in open('/proc/mounts').readlines():
            mnt = re.findall(devnode,l)
            if mnt:
                # print mnt
                dst = 'usage'
                break

    elif f.starstwith('sr'):
        devnode= '/dev/%s' %(f)
        dtype = 'cdrom'
        dst = 'noMedia'
        args = ['lcdromid','devnode']
        Popen(args,stdout=PIPE)
        Popen.wait()
        ret = Popen.communicate()
        print ret

    print dtype
    print dst
