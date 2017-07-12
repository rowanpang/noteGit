#!/usr/bin/python

import pcap
import dpkt

pc=pcap.pcap()

pc.setfilter('tcp port 80')

for ptime,pdata in pc:
    print ptime,pdata
    p=dpkt.ethernet.Ethernet(pdata)

    if p.data.__class__.__name__=='IP':
        ip='%d.%d.%d.%d'%tuple(map(ord,list(p.data.dst)))

    if p.data.data.__class__.__name__=='TCP':
        if p.data.data.dport==80:
            print p.data.data.data
