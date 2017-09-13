#!/usr/bin/python
# -*- coding: utf-8 -*-

import configobj

def holdSectionVal(section,key):
    print section
    print key
    print

f = './url.txt'
cfg = configobj.ConfigObj(f)

print cfg.keys()
print cfg.sections
print cfg.dict()
print cfg.items()
for k in cfg.sections:
    print k
    print type(cfg[k])
    print cfg[k]
    print cfg.as_list(k)

    if 'entry' in cfg[k].keys():
        print cfg[k].as_list('entry')
    else:
        print '----inValid key-----'

    if 'entryxxxx' in cfg[k].keys():
        print cfg[k].as_list('entryxxxx')
    else:
        print '----inValid key-----'

print cfg.values()

# cfg.walk(holdSectionVal)
# print cfg
