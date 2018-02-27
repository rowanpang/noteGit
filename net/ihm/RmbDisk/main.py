#!/usr/bin/python
#coding:utf-8

import os
import re
import json

from flask import Flask,jsonify
from flask import Blueprint,abort
from subprocess import Popen,PIPE

rootdir =  '/sys/class/block'
lcdromid = os.path.join(os.path.dirname(__file__),'cdrom_id')
# print lcdromid
RmbDiskRoute = Blueprint('RmbDisk', __name__) 

def sizeShred(byts):
    ''' return size str'''
    gb = 1024*1024*1024
    mb = 1024*1024
    kb = 1024

    if byts >= gb:        #G
        size = '%.2fG' %(byts/gb)
    elif byts >= mb:
        size = '%.2fM' %(byts/mb)
    elif byts >= kb:
        size = '%.2fK' %(byts/gb)
    else:
        size = '%s' %(byts)

    return size

def isMounted(devnode):
    for l in open('/proc/mounts').readlines():
        mnt = re.findall(devnode,l)
        if mnt:
            return True

    return False

@RmbDiskRoute.route('/rmbDisk')
def show_info():
    DISK_ST_INSERTED='inserted'
    DISK_ST_NOMEDIA='noMedia'
    DISK_ST_USING='using'

    DISK_TYPE_DISK='disk'
    DISK_TYPE_UDISK='udisk'
    DISK_TYPE_CDROM='cd-dvd'

    ret = {}
    dl=[]
    for f in os.listdir(rootdir):
        disk={}
        dd = os.path.join(rootdir,f)
        if not re.match('^sd[a-z]+$|sr[0-9]+$',f):
            continue
        if open(os.path.join(dd,'removable')).read(1) == '0':
            continue
        disk['size'] = sizeShred(float(open(os.path.join(dd,'size')).read().strip()) * 512)
        dtype = DISK_TYPE_DISK
        if f.startswith('sd'):
            devnode= '/dev/%s[0-9]+' %(f)
            if not os.path.islink(dd):
                pass
            if re.findall('usb[0-9]+\/',os.readlink(dd)):
                dtype = DISK_TYPE_UDISK
            dst = DISK_ST_INSERTED
            if isMounted(devnode):
                dst = DISK_ST_USING

        elif f.startswith('sr'):
            devnode= '/dev/%s' %(f)
            dtype = DISK_TYPE_CDROM
            dst = DISK_ST_NOMEDIA
            args = [lcdromid,devnode]
            p = Popen(args,stdout=PIPE)
            p.wait()
            retdic = dict(s.split('=') for s in p.communicate()[0].splitlines())
            if 'ID_CDROM_MEDIA' in retdic.keys() and retdic['ID_CDROM_MEDIA']:
                dst = DISK_ST_INSERTED
            if isMounted(devnode):
                dst = DISK_ST_USING

        disk['type'] = dtype
        disk['dst'] = dst
        # print disk
        dl.append(disk)

    ret['num'] = len(dl)
    ret['info'] = dl

    return json.dumps(ret)

if __name__ == '__main__':
    # print show_info()
    app = Flask(__name__)
    app.register_blueprint(RmbDiskRoute, url_prefix='')
    app.run(host = '0.0.0.0', port=8080,debug = True)
