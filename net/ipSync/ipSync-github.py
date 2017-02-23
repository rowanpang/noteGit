#!/usr/bin/env python

import os,httplib

def ipWanGet(ipFile):
    host = '1212.ip138.com'
    urlReq = '/ic.asp'
    userAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'
    connKeep = 'close'

    conn = httplib.HTTPConnection(host)
    conn.connect() 
    conn.putrequest("GET",urlReq)
    conn.putheader("User-Agent",userAgent)
    conn.putheader("Accept-Encoding",'gzip,deflate')
    conn.putheader("X-Request-With",'XMLHttpRequest')
    conn.putheader("Connection",connKeep)
    conn.endheaders()

    repbody = conn.getresponse().read().decode('gbk').encode('utf8')
    print repbody
    conn.close()    
    
    if not os.path.exists(os.path.dirname(ipFile)):
        os.mkdir(os.path.dirname(ipFile))
    open(ipFile,'w+').write(repbody)

def gitToGithub(file):
    os.system('git add %s' %file)
    os.system('git commit -v -m "update ip files %s" %s' %(file,file))
    os.chdir(os.path.dirname(file))
    os.system('git push')

if __name__ == '__main__':
    ipFile = '/tmp/rowan/github/net/ipSync/ipWan-%s' %os.uname()[1]
    ipWanGet(ipFile)
    gitToGithub(ipFile)
