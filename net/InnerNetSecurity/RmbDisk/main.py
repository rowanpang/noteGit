#!/usr/bin/python
#coding:utf-8
from flask import Flask,jsonify
from flask import Blueprint,abort

import os
import re
from subprocess import Popen,PIPE

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
    ret['a'] = 'aaaaaaaaaaa'
    ret['b'] = 'bbbbbbbbbbb'
    for f in os.listdir(rootdir):
        dd = rootdir + '/' + f + '/'
        if not re.match('^sd[a-z]+$|sr[0-9]+$',f):
            continue
        if open(dd + 'removable').read(1) == '0':
            continue
        ret['size'] = sizeShred(float(open(dd + 'size').read().strip()) * 512)

        if f.startswith('sd'):
            devnode= '/dev/%s[0-9]+' %(f)
            dtype = 'disk'
            if not os.path.islink(dd[:-1]):
                pass
            if re.findall('usb[0-9]+\/',os.readlink(dd[:-1])):
                dtype = 'udisk'
            dst = 'inserted'
            if isMounted(devnode):
                dst = 'using'

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
    return jsonify(ret)

rootdir =  '/sys/class/block'
lcdromid = os.path.join(__file__,'cdrom_id')

if __name__ == '__main__':
    app = Flask(__name__)
    app.register_blueprint(RmbDiskRoute, url_prefix='')
    app.run(host = '0.0.0.0', port=8080,debug = True)
