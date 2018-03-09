#!/usr/bin/env python

import urllib
import json
import re
import sockschain
import socket
import os

class Jdprice :
    def __init__(self,url):
        self.url = url
        self._response = urllib.urlopen(self.url)
        self.html = self._response.read()

    def get_productinfo(self):
        product_re = re.compile('product:.*\n.*skuid:.*')
        product_info = product_re.findall(self.html)[0]
        return product_info
    
    def get_productinfo_skuid(self):
        product_info = self.get_productinfo()
        skuid = re.search('skuid:\s+([0-9]+),',product_info)
        if skuid:
            return skuid.group(1)
        else:
            return None
    
    def get_product_price(self):
        price = None
        skuid = self.get_productinfo_skuid()
        url = 'http://p.3.cn/prices/mgets?skuIds=J_' + skuid + '&type=1'
        price_json = json.load(urllib.urlopen(url))[0]
        if price_json['p']:    
            price = price_json['p']
            return price

class Productfile :
    def __init__(self,file):
        self.filename = file
        self.file = open(self.filename,'r')
        indexline = self.file.readline() 
        self.prices = list()
        self.newhead = list()
        while re.match('^\s*#',indexline):    # ignore the comment
            self.newhead.append(indexline)
            indexline = self.file.readline()
        
        from datetime import date
        i = 0
        self.row_today = -1
        self.row_url = -1

        for indexstr in indexline.split():
            if indexstr == 'jd-url':
                self.row_url = i
            if indexstr == date.today().strftime("%Y-%m-%d"):
                self.row_today = i
                break
            i += 1
        if self.row_today == -1:
            self.row_today = i;

        if self.row_today == 2:
            self.skip = 3
        else:
            self.skip = 2

        indexline = indexline.rstrip('\n') + self.skip * '\t' + date.today().strftime("%Y-%m-%d") + '\n'
        self.newhead.append(indexline)
        self.products=self.file.readlines()
        self.file.close()

    def geturls(self):
        urls = list()
        for product in self.products:
            if not product.startswith("#"):
                urls.append(product.split()[self.row_url])
        return urls

    def pricetoday(self,price):
        self.prices.append(price) 

    def writetmp(self):
        self.filenametmp = self.filename + '.tmp'
        tmp = open(self.filenametmp,'w+')
        tmp.writelines(self.newhead)
        it = iter(self.prices)
        for product in self.products:
            if product.startswith("#"):
                tmp.write(product)
            else:
                tmp.write(product.rstrip("\n") + self.skip * "\t") 
                tmp.write(it.next())
                tmp.write("\n")
        tmp.flush()
        tmp.close()

    def printinfo(self):
        print self.row_url
        print self.row_today
        print self.newhead
        print self.products
        print type(self.products[0])
        print self.geturls()    
    
def setsocks():
    sockschain.setdefaultproxy(sockschain.PROXY_TYPE_SOCKS5,"127.0.0.1",8080)
    socket.socket = sockschain.socksocket

if __name__ == '__main__':
    setsocks()
    file = "./product.txt"
    pf = Productfile(file)
    pf.printinfo()
    for url in pf.geturls():
        jp = Jdprice(url)
        pf.pricetoday(jp.get_product_price())
    pf.writetmp()

    os.unlink(file)
    os.rename(pf.filenametmp,pf.filename)
