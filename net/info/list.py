#!/usr/bin/python

import netifaces

for ifname in netifaces.interfaces():
    ip = None;mac = None
    addrFamily = netifaces.ifaddresses(ifname)
    for k in addrFamily.keys():
        if k == netifaces.AF_LINK:
            mac = addrFamily[k][0]['addr']
        if k == netifaces.AF_INET:
            ipdic = addrFamily[k][0]
            ip = ipdic['addr']
    print "ifname:%s\t\tmac:\'%s\'\t\tip:%s" %(ifname,mac,ip)
