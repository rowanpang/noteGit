#!/usr/bin/python
# -*- coding: utf-8 -*-
a=10
b=0
class selfError(Exception):
    def __init__(self,x="selError test"):
        Exception.__init__(self,x)
        self.x = x
try:
    c=a/b
    print c
except (IOError,ZeroDivisionError),e:
    print e.message
    print type(e)

else:
    print 'no error'


raise selfError()
raise selfError("xxxxxxxxx")
