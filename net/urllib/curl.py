#!/usr/bin/python
# -*- coding: utf-8 -*-

import urllib2
import time

print 'hello'
url = 'https://baidu.com'
req = urllib2.Request(url)
rep = urllib2.urlopen(req)

print rep.read()
