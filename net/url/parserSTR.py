#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys

class parseURL():
    def __init__(self,url):
        self.protocal = ''
        self.host = ''
        self.port = '' 
        self.url = ''
        
        idx2 = url.find('//')
        if idx2 != -1:
            self.protocal = url[:url.index(':')]
            tmp = url[idx2+2:]
        else:
            tmp = url
            
        if tmp: 
            idx1 = tmp.find('/')
            if idx1 != -1:
                domain = tmp[:idx1]
                self.url= tmp[idx1:]
            else:
                domain = tmp
                self.url= '/'

            idx = domain.find(':')
            if idx != -1:
                self.host = domain[0:idx]
                self.port = domain[idx+1:]
            else:
                self.host = domain

    def getinfo(self):
        return self.protocal,self.host,self.port,self.url

if '__main__' == __name__:
    url = "http://ppp.com:8090/mximprove/mxt/scripts/views/MainViewController.js";
    for url in open('./url.txt').readlines():
        print url.strip()
        u = parseURL(url.strip())
        print u.getinfo()
        print
