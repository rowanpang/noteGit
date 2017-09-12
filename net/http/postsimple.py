#!/usr/bin/env /usr/bin/python
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

def svrConnection(dstIp,dstPort = None,srcIp = None,srcPort = None):
    if srcIp is None:
        conn = httplib.HTTPConnection(host=dstIp,port=dstPort)
    else:
        conn = httplib.HTTPConnection(host=dstIp,port=dstPort,source_address=(srcIp,srcPort))

    conn.connect()
    return conn

def svrPostGotRep(conn,urlreq,djson):
    UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0'
    conn.putrequest("POST",urlreq)
    conn.putheader("User-Agent",UserAgent)
    conn.putheader("Accept-Encoding",'gzip,deflate')
    conn.putheader("X-Request-With",'XMLHttpRequest')
    conn.putheader("Content-Type",'application/json; charset=UTF-8')
    conn.putheader("Content-Length",str(len(djson)))
        #必须,如果没有将会出错,不会发送 post request.

    conn.endheaders()
    conn.send(djson)
    rep = conn.getresponse()
    return rep

def main():
    svr = '127.0.0.1'
    port = 8080
    urlreq = '/policyexec'
    dataGetInfo= {
                    'services':
                        [
                            {'id': 1,'name':'http','enable':1},
                            {'id': 2,'name':'http2','enable':11},
                        ]
                        ,
                    'ports':
                        [
                            {'id': 1,'name':'port','enable':1},
                            {'id': 2,'name':'port','enable':11},
                        ]
                    }
    dj = json.dumps(dataGetInfo)

    conn = svrConnection(svr,port)
    rep = svrPostGotRep(conn,urlreq,dj)
    print rep.read()
    conn.close()

    return 0

if __name__ == '__main__':
    main()
