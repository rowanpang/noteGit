#!/usr/bin/python	
# -*- coding: utf-8 -*-

import mechanize  
import cookielib  
from BeautifulSoup import BeautifulSoup  
  
  
br = mechanize.Browser()  
cj = cookielib.LWPCookieJar()  
br.set_cookiejar(cj)##关联cookies  
  
###设置一些参数，因为是模拟客户端请求，所以要支持客户端的一些常用功能，比如gzip,referer等  
br.set_handle_equiv(True)  
br.set_handle_gzip(True)  
br.set_handle_redirect(True)  
br.set_handle_referer(True)  
br.set_handle_robots(False)  
  
br.set_handle_refresh(mechanize._http.HTTPRefreshProcessor(), max_time=1)  
  
###这个是degbug##你可以看到他中间的执行过程，对你调试代码有帮助  
br.set_debug_http(True)  
#br.set_debug_redirects(True)  
#br.set_debug_responses(True)  
  
br.addheaders = [('User-agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.1.11) Gecko/20100701 Firefox/3.5.11')]##模拟浏览器头  
response = br.open('https://baidu.com/')##自己设定一个url  

print response.read()
