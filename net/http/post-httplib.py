#!/usr/bin/env python2.7
#coding: utf-8

import httplib 
import urllib
import base64
import sys

svr = '10.6.6.9'
src0 = '10.200.40.25'  #eth0.2
src1 = '10.200.40.29'  #vth1
src2 = '10.200.40.34'  #vth2
src3 = '10.200.40.37'  #vth3

src = src0
if len(sys.argv) >=2:
	if sys.argv[1] == '1':
		src = src1
	if sys.argv[1] == '2':
		src = src2
	if sys.argv[1] == '3':
		src = src3
print src
urlreq = '/a/ajax.php?tradecode=net_auth&type=User&NewMobile=1'
UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'

conn = httplib.HTTPConnection(host=svr, source_address=(src,6666))
conn.connect()
conn.putrequest("POST",urlreq) 
conn.putheader("User-Agent",UserAgent)
conn.putheader("Accept-Encoding",'gzip,deflate')
conn.putheader("X-Request-With",'XMLHttpRequest')
conn.putheader("Referer",'http://10.6.6.9/a/mobile/auth.html')
#conn.putheader("Connection",'close')

data={'user_name':'',
		'password':'',
		'deviceid':478183,
		'saveuserpass':1 }
userName = 'pangweizhenbj'
pwd = 'inspur#2cPWZ'
data['user_name'] = base64.b64encode(userName)
data['password'] = base64.b64encode(pwd)
dataUrl = urllib.urlencode(data)

conn.putheader("Content-Type",'application/x-www-form-urlencoded; charset=UTF-8')
conn.putheader("Content-Length",str(len(dataUrl)))   #必须，如果没有将会出错，不会发送 post request.
conn.endheaders()
conn.send(dataUrl)
rep = conn.getresponse()

print rep.getheader('Content-Type')
print rep.read()
conn.close()
