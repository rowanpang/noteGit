#!/usr/bin/python
# -*- coding: utf-8 -*-

import urllib2
import time

print 'hello'
req = urllib2.Request('http://www.zjtkgf.com')
rep = urllib2.urlopen(req)
time.sleep(2)

print rep.read()
