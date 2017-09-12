#!/usr/bin/python
# -*- coding: utf-8 -*-

import configobj

def holdSectionVal(section,key):
    print section
    print key
    print

f = './url.txt'
cfg = configobj.ConfigObj(f)

print cfg.sections
print cfg.dict()
print cfg.items()
print cfg.keys()
for k in cfg.iterkeys():
    print k
    print type(cfg[k])
    print cfg[k]
    print cfg[k].as_list('entry')
    print cfg.as_list(k)

print cfg.values()

# cfg.walk(holdSectionVal)
# print cfg
