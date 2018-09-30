#!/usr/bin/python
# coding:utf-8

import sys
import os
import httplib
import time
import ssl
import platform
from distutils.version import LooseVersion

if __name__ == '__main__':
    sys.path.insert(0, os.path.dirname(__file__) + '/../')
from util.ServerInfo import get_server_ip, get_main_localip, get_main_netcard


def svrToken():
    return "inspur123"


def sendGotRep(
        urlreq='/',
        data=None,
        appSubType='json',
        dstIp=None,
        dstPort=19003):
    if not dstIp:
        for i in range(3):
            dstIp = get_server_ip()
            if dstIp:
                break
            time.sleep(0.5)

    if not dstIp:
        errMsg = "no server ip"
        print '%s:%s' % (urlreq, errMsg)
        return None, errMsg

    caCert = os.path.dirname(__file__) + "/ihmCA.cer"
    pyCurVer = platform.python_version()
    pyComVer = '2.7.9'
    try:
        https = False
        https = True
        if https:
            if cmp(LooseVersion(pyCurVer), LooseVersion(pyComVer)) >= 0:
                # pyCurVer >= pyComVer
                # so we need use ssl ctx to disable hostname verify
                # print 'use ssl context'
                ctx = ssl.create_default_context(cafile=caCert)
                ctx.check_hostname = False  # python didn't support ip check

                conn = httplib.HTTPSConnection(
                    host=dstIp, port=dstPort, context=ctx)
            else:
                # print 'just caCert'
                conn = httplib.HTTPSConnection(host=dstIp, port=dstPort)

        else:
            conn = httplib.HTTPConnection(host=dstIp, port=dstPort)
        conn.connect()
    except BaseException:
        errMsg = "connect to server error"
        print '%s:%s%-20s:%s' % (dstIp, dstPort, urlreq, errMsg)
        return None, errMsg

    UserAgent = 'Mozilla/5.0 (X11; Linux x86_64; rv:45.0)\
                 Gecko/20100101 Firefox/45.0'
    method = 'POST' if data else 'GET'
    conn.putrequest(method, urlreq)
    conn.putheader("User-Agent", UserAgent)
    conn.putheader("Accept-Encoding", 'deflate')
    conn.putheader("X-Request-With", 'XMLHttpRequest')
    if data:
        conn.putheader(
            "Content-Type",
            'application/%s; charset=UTF-8' %
            (appSubType))
        conn.putheader("Content-Length", str(len(data)))
    conn.endheaders()

    if data:
        conn.send(data)
    try:
        rep = conn.getresponse()
        if not rep or rep.status != 200:
            errMsg = "send data error"
            print '%s:%s%-20s:%s' % (dstIp, dstPort, urlreq, errMsg)
            pass
        else:
            errMsg = "send data ok"
            # print '%s:%s%-20s:%s' %(dstIp,dstPort,urlreq,errMsg)
            pass
    except Exception as e:
        errMsg = "send data error: %s" % (e)
        print '%s:%s%-20s:%s' % (dstIp, dstPort, urlreq, errMsg)
        return None, errMsg

    return rep, 'ok'


if __name__ == '__main__':
    rep, msg = sendGotRep('/', dstIp='www.baidu.com', dstPort=80)
    print rep, msg
