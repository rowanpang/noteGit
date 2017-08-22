#!/usr/bin/python
# -*- coding: utf-8 -*-

import urlobject
# ref https://urlobject.readthedocs.io/en/latest/quickstart.html

for l in open('./url.txt').readlines():
    uobj = urlobject.URLObject(l.strip())
    print uobj
    print 'scheme',uobj.scheme
    print 'hostname',uobj.hostname
    print 'port',uobj.port
    print 'defport',uobj.default_port
    print 'path',uobj.path
    print 
