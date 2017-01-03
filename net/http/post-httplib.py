#!/usr/bin/env /usr/local/bin/python2.7
#coding: utf-8
#Usage: 
    # ./post-httplib.py     or 
    # ./post-httplib.py eth0 eth1

import httplib,struct,fcntl,socket,os
import urllib
import base64
import sys
import json
import getpass

def svrConnection(svr,srcIp = None,srcPort = 6666):
    if srcIp is None:
        conn = httplib.HTTPConnection(host=svr)
    else:
        conn = httplib.HTTPConnection(host=svr, source_address=(srcIp,srcPort))

    conn.connect()
    return conn
    
def svrPostGotRep(conn,urlreq,ref,connkeep,data):
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

authenUser = ''
def ifAuthenGetUser():
    global authenUser
    authenUserDefault = 'pangweizhenbj'
    if len(authenUser) != 0: 
        return authenUser

    authenUser = raw_input('Input username to be authen[%s]: ' %authenUserDefault);    
    if len(authenUser) == 0:
        authenUser = authenUserDefault

    return authenUser

authenPWD = ''
def ifAuthenGetPWD():
    global authenPWD
    if len(authenPWD) != 0:
        return authenPWD

    authenPWD = getpass.getpass();
    while len(authenPWD) == 0:
        authenPWD = getpass.getpass();

    return authenPWD
    
def ifAuthen(svr,ifSpec = None):
    ret = False
    print 'Authen for: %s' %ifSpec[0]
    userName = ifAuthenGetUser()
    pwd = ifAuthenGetPWD()
    print userName
    print pwd

    if ifSpec is None:
        conn = svrConnection(svr)
    else:
        conn = svrConnection(svr,ifSpec[1])

    urlreq = '/a/ajax.php?tradecode=getdeviceinfoprocess&gettype=ipgetmac'
    referer = 'http://10.6.6.9/a/mobile/wel.html'
    connkeep = 'keep-alive'
    dataGetInfo={'newMobile':1,
                    'is_guest':0,
                    'os_platform':'Linux',
                    'browser':'Firefox',
                    'UrlAscid':''
                }
    infoRepDict,isJson = repParser(svrPostGotRep(conn,urlreq,referer,connkeep,dataGetInfo))
    if not isJson:
        print 'error msg %s' % infoRepDict
        conn.close()
        return ret

    urlreq = '/a/ajax.php?tradecode=net_auth&type=User&NewMobile=1'
    referer = 'http://10.6.6.9/a/mobile/auth.html'
    connkeep = 'close'
    dataAuth={'user_name':'',
            'password':'',
            'deviceid':'',
            'saveuserpass':1 }
    dataAuth['user_name'] = base64.b64encode(userName)
    dataAuth['password'] = base64.b64encode(pwd)
    dataAuth['deviceid'] = infoRepDict['DeviceID']

    authRepDict,isJson = repParser(svrPostGotRep(conn,urlreq,referer,connkeep,dataAuth))
    # print '----isJson:%d---' % isJson
    if isJson == True:
        if authRepDict['IsDisabled'] == '0':
            print 'successful!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
            ret = True
        else:
            print 'error!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
    else:
        print 'error!! Auth IP:%s,MAC:%s !' % (infoRepDict['IP'],infoRepDict['Mac'])
        print 'error msg %s' % authRepDict

    conn.close()
    return ret

gwsPromptCache = {} 
def ifGetGwsPrompt(ifname):
    global gwsPromptCache    
    try:
        return gwsPromptCache[ifname]
    except KeyError,key:
        print '%s,not in prompt cache' %key

    while True:
        gw = raw_input('Auto get err,please manual input gateway for %s: ' %ifname)
        try:
            socket.inet_aton(gw)
            break
        except socket.error,e:
            print 'gw format error: %s,again' %e
            continue
    
    gwsPromptCache[ifname] = gw
    return gw

def ifGetGws(ifSpecs):
    gwsAuto = {}
    rts = os.popen('ip route').readlines()
    for rt in rts:
        rtl = rt.split()
        # print rtl[0] + ' ' + rtl[4] + ' ' 
        # print ifSpecs.keys()
        if rtl[0] == 'default' and rtl[4] in ifSpecs.keys() and (not rtl[4] in  gwsAuto.keys()):
            gwsAuto[rtl[4]] = rtl[2]
    # print gwsAuto
    
    for ifname in ifSpecs.keys():
        if ifname in gwsAuto.keys():
            continue
        else:   #auto find gateWay for ifname field
            gwsAuto[ifname] = ifGetGwsPrompt(ifname)

    return gwsAuto
    
def ifNamesConfirm(default):
    ifnames = []
    if len(sys.argv) > 1:
        for arg in sys.argv:
            if arg in default:
                ifnames.append(arg)
    if len(ifnames) < 1:
        print 'less than 1'
        ifnames = default
    return ifnames

def ifGetAddr(ifnames):
    ifSpecs = {}
    for ifname in ifnames:
        s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        ifreq = fcntl.ioctl(s.fileno(),0x8915, #SIOCGIFADDR
                    struct.pack('256s',ifname))
        ip = socket.inet_ntoa(ifreq[20:24])
        s.close()
        ifSpecs[ifname] = ip
    return ifSpecs


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

def ifAuthens(svr,ifSpecs = None):
    if ifSpecs is None:
        ifAuthen(svr)
    else:
        for ifSpec in ifSpecs.items():
            ifAuthen(svr,ifSpec)

#for ifSpecs[x]=value ,key is ifname,value is ip for ifname.
#for ifSpec will be a (key, value) pair for ifSpecs.
# ifSpec[0]:ifname 
# ifSpec[1]:ifsrc 
def main():
    svr = '10.6.6.9'
    if os.getuid() != 0:
        print 'need run as root,exit!'
        exit()
    # ifnamesDefault = ('eth0.2','vth1','vth2','vth3')
    ifnamesDefault = ('wlan0','bridged')
    ifnames = ifNamesConfirm(ifnamesDefault)        #default ifnames to authen.
    ifSpecs = ifGetAddr(ifnames)
    rtCmd(ifSpecs,ifGetGws(ifSpecs))
    ifAuthens(svr,ifSpecs)
    rtCmd(ifSpecs,ifGetGws(ifSpecs),build = False)

# src0 = '10.200.40.25'  #eth0.2
# src1 = '10.200.40.29'  #vth1
# src2 = '10.200.40.34'  #vth2
# src3 = '10.200.40.37'  #vth3
if __name__ == '__main__':
    main()
