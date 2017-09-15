#!/usr/bin/python

import pyroute2
from pyroute2 import iproute

ipr = iproute.IPRoute()
links = ipr.get_links()
for  l in links:
    print l.get_attr('IFLA_IFNAME')

# print ipr.get_default_routes()
# print ipr.get_routes()
