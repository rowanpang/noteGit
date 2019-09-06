#!/usr/bin/python
# -*- coding: utf-8 -*-

print "string-1 %s" % ("100")
print "string-1 %s,%d" % ("length",10)
print "string-1 %(k1)s,%(k2)d" % {"k1":"k1String","k2":300}

print "string a+b: %s" %("aaaa" + "bbbb")
c="cccccc"
d="dddddd"
print "string c+d: %s" %(c+d)
