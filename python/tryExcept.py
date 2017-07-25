#!/usr/bin/python
# -*- coding: utf-8 -*-
a=10
b=0
try:
    c=a/b
    print c
except (IOError,ZeroDivisionError),e:
    print e.message
    print type(e)

else:
    print 'no error'

print "done"
