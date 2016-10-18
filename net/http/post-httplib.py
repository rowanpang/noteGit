#!/usr/bin/env /usr/local/bin/python2.7
#coding: utf-8
#Usage: 
	# ./post-httplib.py	or 
	# ./post-httplib.py eth0 eth1


import httplib,struct,fcntl,socket,os
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
	return rep

def repstr2dict(repbody,coding,part = True):
	# print repbody
	if part:
		jsonstr = repbody.replace(', "Tel','}$')
	jsonstr = jsonstr[jsonstr.find('{'):jsonstr.find('$')].decode('gbk').encode('utf8').\
			replace(' ','"',1).replace(':','":',1).replace('\'','"').replace('(','-').\
			replace(')','-')
	repdict = json.loads(jsonstr)
	return repdict

def repParser(rep):
	isJson = True
	repbody = rep.read()
	if repbody.find('__res = {') == -1:
		isJson = False
	# print isJson
	# print rep.getheader('Content-Type')
	coding = rep.getheader('Content-Type').split()[1].split('=')[1].lower()
	# print coding

	if isJson is True:
		repdict = repstr2dict(repbody,coding)		
	else:
		repdict = repbody.decode(coding).encode('utf8')

	return repdict,isJson
	
def ifAuthen(svr,ifSpec = None):
	ret = False

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
	infoRepDict,isJson = repParser(svrPostGotRep(urlreq,referer,connkeep,dataGetInfo))
	# print infoRepDict
	# print '----isJson:%d---' % isJson
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

	authRepDict,isJson = repParser(svrPostGotRep(urlreq,referer,connkeep,dataAuth))
	# print '----isJson:%d---' % isJson
	# exit()
	if isJson == True:
		if authRepDict['IsDisabled'] == '0':
			print 'successful!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
			ret = True
		else:
			print 'error!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
			ret = False
	else:
		ret = False
		print 'error!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
		print 'error msg %s' % authRepDict

	conn.close()
	return ret

def ifGetGws(ifSpecs):
	gwsAuto = {}
	rts = os.popen('ip route').readlines()
	for rt in rts:
		rtl = rt.split()
		# print rtl[0] + ' ' + rtl[4] + ' ' 
		# print ifSpecs.keys()
		if rtl[0] == 'default' and rtl[4] in ifSpecs.keys():
			gwsAuto[rtl[4]] = rtl[2]
	# print gwsAuto
	return gwsAuto
	

def rtCmd(ifSpecs,gws,build = True):
	if build == True:
		cmd = 'add'
	else:
		cmd = 'del'
	
	rtTable = 150
	pref = 32000
	for ifSpec in ifSpecs.items():
		cmdStr = 'ip route %s to default via %s dev %s table %d' % (cmd,gws[ifSpec[0]],ifSpec[0],rtTable)
		# print cmdStr
		os.system(cmdStr)
		cmdStr = 'ip rule %s from %s pref %d table %d' %(cmd,ifSpec[1],pref,rtTable)
		# print cmdStr
		os.system(cmdStr)
		pref -= 1
		rtTable -= 1

# src0 = '10.200.40.25'  #eth0.2
# src1 = '10.200.40.29'  #vth1
# src2 = '10.200.40.34'  #vth2
# src3 = '10.200.40.37'  #vth3
def main():
	svr = '10.6.6.9'
	ifnames = []
	# ifnamesDefault = ('eth0.2','vth1','vth2','vth3')
	ifnamesDefault = ('wlan0','bridged')
	if len(sys.argv) > 1:
		for arg in sys.argv:
			if arg in ifnamesDefault:
				ifnames.append(arg)

	if len(ifnames) < 1:
		print 'less than 1'
		ifnames = ifnamesDefault

	# print ifnames
	# exit()
	ifSpecs = {}
	for ifname in ifnames:
		# print ifname
		ifSpecs[ifname] = ifGetAddr(ifname)
	# print ifSpecs
	# exit()
	
	rtCmd(ifSpecs,ifGetGws(ifSpecs))
	for ifSpec in ifSpecs.items():
		print ifSpec[0]
		# print ifSpec[1]
		ifAuthen(svr,ifSpec)
	rtCmd(ifSpecs,ifGetGws(ifSpecs),False)

if __name__ == '__main__':
	main()


