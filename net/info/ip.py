#!/usr/bin/env python
#coding:UTF-8
# ref http://blog.chinaunix.net/uid-25885064-id-3447914.html

import socket,struct,fcntl

def getIpAddr(ifname):
	s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	ifreq = fcntl.ioctl(s.fileno(),0x8915, #SIOCGIFADDR 
			struct.pack('256s',ifname))
	ip = socket.inet_ntoa(ifreq[20:24])
	s.close()
	print ip
	return ip
	
ips = []
ips.append(getIpAddr('lo'))
ips.append(getIpAddr('bridged'))
ips.append(getIpAddr('wlan0'))

print ips

for ip in ips:
	print ip
