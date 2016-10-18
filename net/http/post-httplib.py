#!/usr/bin/env python2.7
#coding: utf-8

import httplib,struct,fcntl,socket
import urllib
import base64
import sys
import json


def ifGetAddr(ifname):
	s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	ifreq = fcntl.ioctl(s.fileno(),0x8915, #SIOCGIFADDR
				struct.pack('256s',ifname))
	ip = socket.inet_ntoa(ifreq[20:24])
	s.close()
	return ip

def svrConnection(svr,srcIp = None,srcPort = 6666):
	global conn
	if srcIp is None:
		conn = httplib.HTTPConnection(host=svr)
	else:
		conn = httplib.HTTPConnection(host=svr, source_address=(srcIp,srcPort))

	conn.connect()
	return conn
	
def svrPostGotRep(urlreq,ref,connkeep,data):
	UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'
	conn.putrequest("POST",urlreq) 
	conn.putheader("User-Agent",UserAgent)
	conn.putheader("Accept-Encoding",'gzip,deflate')
	conn.putheader("X-Request-With",'XMLHttpRequest')
	conn.putheader("Referer",ref)
	conn.putheader("Connection",connkeep)

	dataUrl = urllib.urlencode(data)
	conn.putheader("Content-Type",'application/x-www-form-urlencoded; charset=UTF-8')
	conn.putheader("Content-Length",str(len(dataUrl)))   #必须,如果没有将会出错,不会发送 post request.
	conn.endheaders()
	conn.send(dataUrl)
	rep = conn.getresponse()
	return rep.read()

def repstr2dict(repbody,part = True):
	# print repbody
	if part:
		jsonstr = repbody.replace(', "Tel','}$')
	jsonstr = jsonstr[jsonstr.find('{'):jsonstr.find('$')].decode('gbk').encode('utf8').\
		replace(' ','"',1).replace(':','":',1).replace('\'','"').replace('(','-').\
		replace(')','-')
	repdict = json.loads(jsonstr)
	return repdict

def ifAuthen(svr,ifSpec = None):
	if ifSpec is None:
		svrConnection(svr)
	else:
		svrConnection(svr,ifSpec[1])

	urlreq = '/a/ajax.php?tradecode=getdeviceinfoprocess&gettype=ipgetmac'
	referer = 'http://10.6.6.9/a/mobile/wel.html'
	connkeep = 'keep-alive'
	dataGetInfo={'newMobile':1,
					'is_guest':0,
					'os_platform':'Linux',
					'browser':'Firefox',
					'UrlAscid':''
				}
	infoRepDict = repstr2dict(svrPostGotRep(urlreq,referer,connkeep,dataGetInfo))
	# print infoRepDict
	urlreq = '/a/ajax.php?tradecode=net_auth&type=User&NewMobile=1'
	referer = 'http://10.6.6.9/a/mobile/auth.html'
	connkeep = 'close'
	dataAuth={'user_name':'',
			'password':'',
			'deviceid':'',
			'saveuserpass':1 }
	userName = 'pangweizhenbj'
	pwd = 'inspur#2cPWZ'
	dataAuth['user_name'] = base64.b64encode(userName)
	dataAuth['password'] = base64.b64encode(pwd)
	dataAuth['deviceid'] = infoRepDict['DeviceID']

	authRepDict = repstr2dict(svrPostGotRep(urlreq,referer,connkeep,dataAuth))
	# print authRepDict['IsDisabled']
	if authRepDict['IsDisabled'] == '0':
		print 'successful!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
	else:
		print 'error!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
	
	conn.close()


src0 = '10.200.40.25'  #eth0.2
src1 = '10.200.40.29'  #vth1
src2 = '10.200.40.34'  #vth2
src3 = '10.200.40.37'  #vth3

src = '192.168.1.100'
if len(sys.argv) >=2:
	if sys.argv[1] == '1':
		src = src1
	if sys.argv[1] == '2':
		src = src2
	if sys.argv[1] == '3':
		src = src3
print src

def main():
	svr = '10.6.6.9'
	# ifs = ('eth0.2','vth1','vth2','vth3')
	ifs = ('wlan0',)
	ips = {}
	for ifname in ifs:
		print ifname
		ips[ifname] = ifGetAddr(ifname)

	for ifSpec in ips.items():
		print ifSpec[0]
		print ifSpec[1]
		ifAuthen(svr,ifSpec)

if __name__ == '__main__':
	main()

