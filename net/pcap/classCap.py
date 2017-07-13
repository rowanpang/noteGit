#!/usr/bin/python

import pcap
import dpkt
import time
import signal
import collections 
try:
    from http_parser.parser import HttpParser
except ImportError:
    from http_parser.pyparser import HttpParser

class frameParser:
    def __init__(self,verbose = False):
        self.verbose = False

        self.frameTime = ''
        self.ipdst = ''
        self.ipsrc = ''

        self.dport = ''
        self.sport = ''

        self.ishttp = False
        self.ishttpRequest = False
        self.url = ''
        self.method = ''
        self.proVer = ''
        self.host = ''

    def setVerbose(self,onOff):
        self.verbose = bool(onOff)

    def parserEth(self,pkgETH):
        return pkgETH.data

    def parserIP(self,pkgIP):
        self.ipdst = '%d.%d.%d.%d'%tuple(map(ord,list(pkgIP.dst)))
        self.ipsrc = '%d.%d.%d.%d'%tuple(map(ord,list(pkgIP.src)))
        if self.verbose:
            print 'ip src %s' %ipsrc
            print 'ip dst %s' %ipdst
        return pkgIP.data

    def parserTCP(self,pkgTCP):
        self.dport = pkgTCP.dport
        self.sport = pkgTCP.sport

        if self.verbose:
            print 'tcp sport %s' %pkgTCP.sport
            print 'tcp dport %s' %pkgTCP.dport
            print 'tcp data offset %s' %pkgTCP.off
            print 'tcp data offset_x2 %s' %hex(pkgTCP.off_x2)
            print 'tcp lenpkg %s' %len(pkgTCP)
            print 'tcp has payload %s'  %((pkgTCP.off_x2>>4)*4 != len(pkgTCP))
        return pkgTCP.data

    def isHttpRequest(self,payload):
        if isinstance(payload,str) and (payload.find(' HTTP/1.') != -1):
            return True
        else:
            return False

    def isHttpResponse(self,payload):
        if isinstance(payload,str) and payload.startswith('HTTP/1.'):
            return True
        else:
            return False
        
    def parserHttp(self,pkgHTTP,isRequest):
        if self.verbose:
            print '------ http %s-----' %('request' if isRequest else 'response')
        self.ishttp=True
        self.ishttpRequest = bool(isRequest)
        parser = HttpParser()
        recvLen = len(pkgHTTP)
        nparsed = parser.execute(pkgHTTP,recvLen)
        assert nparsed == recvLen

        headers = collections.OrderedDict() 
        if parser.is_headers_complete():
            headers = parser.get_headers()
            self.url = parser.get_url()
            self.method = parser.get_method()
            self.proVer = parser.get_version()

            if self.verbose:
                print headers
                print self.url
                print self.method
                print self.proVer
    
        if isRequest:
            self.host = headers['Host']
            if self.verbose:
                print self.host

    def doParser(self,ftime,fdata):
        self.frameData = fdata
        self.frameTime = ftime
        pkgETH = dpkt.ethernet.Ethernet(self.frameData)
        payload = self.parserEth(pkgETH)
        if payload.__class__.__name__=='IP':
            pkgIP = payload
            payload = self.parserIP(pkgIP)

            if payload.__class__.__name__=='TCP':
                pkgTCP = payload
                payload = self.parserTCP(pkgTCP)

                if len(payload) > 0:
                    if pkgTCP.sport == 80:
                        if self.isHttpResponse(payload):
                            self.parserHttp(payload,False)
                        
                    if pkgTCP.dport == 80:
                        if self.isHttpRequest(payload):
                            self.parserHttp(payload,True)

                    if pkgTCP.dport == 21:
                        print 'ftp-----'
                else:
                    if self.verbose:
                        print '--------tcp no payload-------'

sniffer=pcap.pcap()
sniffer.setfilter('tcp port 80 or port 21')
fparser = frameParser()

for frameTime,frame in sniffer:
    fparser.doParser(frameTime,frame)
    type(frameTime)
    print '%.2f %s %s' %(fparser.frameTime,fparser.ipsrc,fparser.ipdst)


