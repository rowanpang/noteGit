#!/usr/bin/env python 
# -*- coding: utf-8 -*-

import socket
import httplib 
import sys
import re
import time
import urlparse
import binascii

#对port的处理不如urlobj
UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'

for l in open('./url.txt').readlines():

    if l.strip().startswith('#'):
        continue

    o = urlparse.urlparse(l.strip())
    print o
    print o.port
    
    o = urlparse.urlsplit(l.strip())

    print o
    print o.port
    
    xmlrpc = '%s://%s/xmlrpc' %(o.scheme,o.netloc)
    wpadmin = '%s://%s/wp-admin' %(o.scheme,o.netloc)
    wpcontent = '%s://%s/wp-content' %(o.scheme,o.netloc)
    # print xmlrpc
    # print wpadmin
    # print wpcontent
