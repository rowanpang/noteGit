#!/usr/bin/python

import ssl,socket
import pprint
from ssl import Purpose

print ssl.get_default_verify_paths()

print ssl.get_server_certificate(("www.baidu.com",443))

ctx = ssl.create_default_context(Purpose.SERVER_AUTH)
certs = ctx.get_ca_certs()
for cert in certs:
    pprint.pprint(cert['subject'])

ctx.check_hostname = False

scon = ctx.wrap_socket(socket.socket(socket.AF_INET),server_hostname = "sss")
scon.connect(("www.baidu.com",443))
cert = scon.getpeercert()
# pprint.pprint(cert)
scon.sendall(b"HEAD / HTTP/1.1\r\n Host:baidu.com\r\n\r\n")
pprint.pprint(scon.recv(1024).split(b"\r\n"))





scon.close()

