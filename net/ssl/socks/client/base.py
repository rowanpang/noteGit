#!/usr/bin/python

import ssl,socket
import pprint
from ssl import Purpose

ctx = ssl.create_default_context(Purpose.SERVER_AUTH)
ctx.check_hostname = False

scon = ctx.wrap_socket(socket.socket(socket.AF_INET),server_hostname = "sss")
scon.connect(("www.baidu.com",443))
cert = scon.getpeercert()
# pprint.pprint(cert)
scon.sendall(b"HEAD / HTTP/1.1\r\n Host:baidu.com\r\n\r\n")
pprint.pprint(scon.recv(1024).split(b"\r\n"))
scon.close()

scon = ctx.wrap_socket(socket.socket(socket.AF_INET),server_hostname = "sss")
scon.connect(("www.360.com",443))
cert = scon.getpeercert()
# pprint.pprint(cert)
scon.sendall(b"GET / HTTP/1.0\r\n Host: www.360.com\r\n\r\n")
pprint.pprint(scon.recv(1024).split(b"\r\n"))
scon.close()
