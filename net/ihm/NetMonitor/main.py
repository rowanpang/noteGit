#!/usr/bin/python
#coding:utf-8
import sys
import os
sys.path.insert(0,os.path.dirname(__file__) + '/site-packages')
import ctypes
ctypes.cdll.LoadLibrary(os.path.dirname(__file__) + '/site-packages/libpcap/libpcap.so')

from flask import Flask, jsonify
from flask import Blueprint, abort
import signal
import time
import re
import collections
import Queue
import json
import threading

import pcap
import dpkt

try:
    from http_parser.parser import HttpParser
except ImportError:
    from http_parser.pyparser import HttpParser

class frameParser:
    PRO_HTTP = "HTTP"
    PRO_FTP = "FTP"
    def __init__(self,verbose = False):
        self.verbose = False
        self.reset()

    def reset(self):
        self.frameTime = ''
        self.ipdst = ''
        self.ipsrc = ''
        self.portd = ''
        self.ports = ''
        self.protocol= ''
        self.ishttpRequest = False
        self.url = ''
        self.method = ''
        self.proVer = ''
        self.host = ''

    def setVerbose(self,onOff):
        self.verbose = bool(onOff)

    def getFrameTime(self):
            return self.frameTime

    def getProtocol(self):
        if self.protocol == self.PRO_HTTP:
            return self.PRO_HTTP + '/' + str(self.proVer[0]) + '.' + str(self.proVer[1])
        if self.protocol == self.PRO_FTP:
            return self.PRO_FTP

    def getIPSrc(self):
        return self.ipsrc

    def getIPDst(self):
        return self.ipdst

    def getMethod(self):
        if self.protocol == self.PRO_HTTP and self.ishttpRequest:
            return self.method
        return '-'

    def getFullUrl(self):
        if self.protocol == self.PRO_HTTP and self.ishttpRequest:
            return 'http://'+self.host+self.url
        return '-'

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
        self.portd = pkgTCP.dport
        self.ports = pkgTCP.sport

        if self.verbose:
            print 'tcp ports %s' %pkgTCP.sport
            print 'tcp portd %s' %pkgTCP.dport
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
        self.protocol=self.PRO_HTTP
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
        self.reset()
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

                    if pkgTCP.dport == 21 or pkgTCP.sport == 21:
                        self.protocol = self.PRO_FTP
                else:
                    if self.verbose:
                        print '--------tcp no payload-------'

def startMonitor(q):
    sniffer=pcap.pcap()
    sniffer.setfilter('tcp port 80 or port 21')
    fparser = frameParser(True)
    try:
        for frameTime,frame in sniffer:
            rec = {}
            if frame == None:
                continue
            fparser.doParser(frameTime,frame)
            proc = fparser.getProtocol()
            if proc == None:
                continue
            rec['time'] = fparser.getFrameTime()
            rec['pro'] = fparser.getProtocol()
            rec['ipsrc'] = fparser.getIPSrc()
            rec['ipdst'] = fparser.getIPDst()
            rec['method'] = fparser.getMethod()
            rec['url'] = fparser.getFullUrl()
            # print rec
            q.put(rec)
    except(KeyboardInterrupt),x:
        os.kill(os.getpid(), signal.SIGTERM)

NetMonitorRoute= Blueprint('NetMonitor', __name__)

@NetMonitorRoute.route('/getrecords')
def netRecords():
    global q
    infolist = []
    while not q.empty():
        rec = q.get()
        infolist.append(rec)

    ret = {}
    ret['totalNum'] = len(infolist)
    ret['netInfo'] = infolist

    return json.dumps(ret)

def init():
    # print 'thread with pid %i' %(threading.currentThread().ident)
    monitor = threading.Thread(target=startMonitor,name="netMonitoring",args=(q,))
    # print 'new thread with pid %i' %(monitor.ident)
    monitor.setDaemon(True)
    monitor.start()

q = Queue.Queue()
init()

if __name__ == "__main__":
    app = Flask(__name__)
    app.register_blueprint(NetMonitorRoute, url_prefix='')
    app.run(host = '0.0.0.0', port=8080,debug = False)
