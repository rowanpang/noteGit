#!/usr/bin/env python 
import httplib 

host = 'rowan.f3322.net:8888'
urlreq = 'http://news.baidu.com'
# urlreq = 'http://www.baidu.com' 
# urlreq = 'http://www.jd.com' 
# urlreq = 'http://jd.com:80' 

UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'

conn = httplib.HTTPConnection(host)
conn.connect()
conn.putrequest("GET",urlreq) 
conn.putheader("User-Agent",UserAgent)
conn.endheaders()
rep = conn.getresponse()
print rep.read().decode('gbk').encode('utf8')
conn.close()

