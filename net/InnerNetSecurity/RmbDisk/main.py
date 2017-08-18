#!/usr/bin/python
#coding:utf-8

import os
import re

from flask import Flask,jsonify
from flask import Blueprint,abort
from subprocess import Popen,PIPE

rootdir =  '/sys/class/block'
lcdromid = os.path.join(os.path.dirname(__file__),'cdrom_id')
print lcdromid
RmbDiskRoute = Blueprint('RmbDisk', __name__) 

@RmbDiskRoute.route('/rmbDisk')
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


def show_info():
    ret={}
    for f in os.listdir(rootdir):
        dd = os.path.join(rootdir,f)
        if not re.match('^sd[a-z]+$|sr[0-9]+$',f):
            continue
        if open(os.path.join(dd,'removable')).read(1) == '0':
            continue
        ret['size'] = sizeShred(float(open(os.path.join(dd,'size')).read().strip()) * 512)

        if f.startswith('sd'):
            devnode= '/dev/%s[0-9]+' %(f)
            dtype = 'disk'
            if not os.path.islink(dd):
                pass
            if re.findall('usb[0-9]+\/',os.readlink(dd)):
                dtype = 'udisk'
            dst = 'inserted'
            if isMounted(devnode):
                dst = 'using'

        elif f.startswith('sr'):
            devnode= '/dev/%s' %(f)
            dtype = 'cdrom'
            dst = 'noMedia'
            args = [lcdromid,devnode]
            p = Popen(args,stdout=PIPE)
            p.wait()
            retdic = dict(s.split('=') for s in p.communicate()[0].splitlines())
            if 'ID_CDROM_MEDIA' in retdic.keys() and retdic['ID_CDROM_MEDIA']:
                dst = 'inserted'
            if isMounted(devnode):
                dst = 'using'

        ret['type'] = dtype
        ret['dst'] = dst

    print ret
    return
    return jsonify(ret)


if __name__ == '__main__':
    print show_info()
    # app = Flask(__name__)
    # app.register_blueprint(RmbDiskRoute, url_prefix='')
    # app.run(host = '0.0.0.0', port=8080,debug = True)
