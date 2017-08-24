#!/usr/bin/env python 
# -*- coding: utf-8 -*-
import httplib 
import zlib
import binascii

host = '120.27.8.109'
port = 80
urlreq = '/?author=1'

UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'
conn = httplib.HTTPConnection(host,port)
conn.connect()
conn.putrequest("GET",urlreq,skip_accept_encoding=True) 
conn.putheader("User-Agent",UserAgent)
conn.putheader("Accept-Encoding",'gzip,deflate,identity')
conn.endheaders()
rep = conn.getresponse()

# print len(rep.read())
print rep.getheaders()
mb = rep.read()
print len(mb)
print binascii.hexlify(mb[:10])
print binascii.hexlify(mb[-10:])

ce = rep.getheader('Content-Encoding')

if ce and ce != 'identity':
    if ce.strip() == 'gzip':
        entity = zlib.decompress(mb,16+zlib.MAX_WBITS)
    else:
        raise

# print rep.read()
conn.close()

