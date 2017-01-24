#!/usr/bin/env python 
import httplib 
import base64

host = 'dav.jianguoyun.com'
UserAgent = 'gvfs/1.28'

userName = 'pangweizhen.2008@hotmail.com'
pwd = 'a97jnj5skdt5rqta'
pwdBase64 = base64.b64encode(userName + ':' + pwd)
pwdAuthen = 'Basic ' + pwdBase64   #'Basic cGFuZ3dlaXpoZW4uMjAwOEBob3RtYWlsLmNvbTphOTdqbmo1c2tkdDVycXRh'

conn = httplib.HTTPConnection(host)
conn.connect()
conn.putrequest("OPTIONS",'/dav') 
conn.putheader("User-Agent",UserAgent)
conn.putheader("Accept-Encoding",'gzip,deflate')
conn.putheader("Connection",'keep-alive')
conn.putheader("Authorization",'Basic %s' %pwdBase64)
conn.endheaders()

rep = conn.getresponse()
print rep.status
print rep.reason
print rep.getheaders()
print rep.read()

urlreq = '/dav/davTest/firstFile.txt'
conn.putrequest("GET",urlreq)
conn.putheader("User-Agent",UserAgent)
conn.putheader("Accept-Encoding",'gzip,deflate')
conn.putheader("Connection",'keep-alive')
conn.putheader("Authorization",pwdAuthen)
conn.endheaders()

rep = conn.getresponse()                                                                
print rep.read()

conn.close()
