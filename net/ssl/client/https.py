#!/usr/bin/python

import httplib
# import ssl,socket
# from ssl import Purpose
import pprint

UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'
host="www.baidu.com"
port=443
method = 'HEAD'
urlreq = '/'

con = httplib.HTTPSConnection(host,port)
con.putrequest(method,urlreq)
con.endheaders()

rep = con.getresponse()
print rep.status
pprint.pprint(rep.getheaders())
