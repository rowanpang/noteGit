#!/usr/bin/python

import pcap
import dpkt
import time
import signal
import collections 
import sys
try:
    from http_parser.parser import HttpParser
except ImportError:
    from http_parser.pyparser import HttpParser

def parserEth(pkgETH):
    return pkgETH.data

def parserframe(frame):
    print 'frame lenght %s' %len(frame)
    print 'frame time %s' %time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(frameTime))

def parserIP(pkgIP):
    ipdst = '%d.%d.%d.%d'%tuple(map(ord,list(pkgIP.dst)))
    ipsrc = '%d.%d.%d.%d'%tuple(map(ord,list(pkgIP.src)))
    print 'ip src %s' %ipsrc
    print 'ip dst %s' %ipdst
    return pkgIP.data

def parserTCP(pkgTCP):
    print 'tcp sport %s' %pkgTCP.sport
    print 'tcp dport %s' %pkgTCP.dport
    print 'tcp data offset %s' %pkgTCP.off
    print 'tcp data offset_x2 %s' %hex(pkgTCP.off_x2)
    print 'tcp lenpkg %s' %len(pkgTCP)
    print 'tcp has payload %s'  %((pkgTCP.off_x2>>4)*4 != len(pkgTCP))
    return pkgTCP.data

def isHttpRequest(payload):
    if isinstance(payload,str) and (payload.find(' HTTP/1.') != -1):
        return True
    else:
        return False

def isHttpResponse(payload):
    if isinstance(payload,str) and payload.startswith('HTTP/1.'):
        return True
    else:
        return False
    
def parserHttp(pkgHTTP,isRequest):
    print '------ http %s-----' %('request' if isRequest else 'response')
    parser = HttpParser()
    recvLen = len(pkgHTTP)
    nparsed = parser.execute(pkgHTTP,recvLen)
    assert nparsed == recvLen

    headers = collections.OrderedDict() 
    if parser.is_headers_complete():
        headers = parser.get_headers()
        print headers
        print parser.get_url()
        print parser.get_method()
        print parser.get_version()

    if isRequest:
        print headers['Host']
        
    else:
        sHost = 'ss'

sniffer=pcap.pcap()
sniffer.setfilter('tcp port 80 or port 21')

frameTime,frame = sniffer.next()
print type(frameTime)
sys.exit()

for frameTime,frame in sniffer:
    parserframe(frame)
    pkgETH = dpkt.ethernet.Ethernet(frame)
    payload = parserEth(pkgETH)

    if payload.__class__.__name__=='IP':
        pkgIP = payload
        payload = parserIP(pkgIP)

        if payload.__class__.__name__=='TCP':
            pkgTCP = payload
            payload = parserTCP(pkgTCP)

            if len(payload) > 0:
                if pkgTCP.sport == 80:
                    if isHttpResponse(payload):
                        parserHttp(payload,False)
                    
                if pkgTCP.dport == 80:
                    if isHttpRequest(payload):
                        parserHttp(payload,True)

                if pkgTCP.dport == 21:
                    print 'ftp-----'
            else:
                print '--------tcp no payload-------'

