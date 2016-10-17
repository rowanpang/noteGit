#!/usr/bin/env python 
import httplib 

host = '10.6.6.9'
urlreq = '/a/mobile/wel.html' # urlreq = "http://10.6.6.9/a/mobile/wel.html"  or
UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'

conn = httplib.HTTPConnection(host)
conn.connect()
conn.putrequest("GET",urlreq) 
conn.putheader("User-Agent",UserAgent)
conn.endheaders()
rep = conn.getresponse()
print rep.read()
conn.close()

