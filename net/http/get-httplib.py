#!/usr/bin/env python
import httplib

host = 'localhost'
urlreq = '/mediaType/a.tt'

host = '10.6.6.9'
urlreq = '/a/mobile/wel.html' # urlreq = "http://10.6.6.9/a/mobile/wel.html"  or
port=80


UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'
conn = httplib.HTTPConnection(host,port=port)
conn.connect()
conn.putrequest("GET",urlreq)
conn.putheader("User-Agent",UserAgent)
conn.endheaders()
rep = conn.getresponse()
print rep.status
print rep.reason
print rep.getheaders()
print rep.read()


urlreq = '/mediaType/b.txt'
conn.putrequest("GET",urlreq)
conn.putheader("User-Agent",UserAgent)
conn.endheaders()
rep = conn.getresponse()
print rep.read()
print rep.getheaders()

conn.close()

