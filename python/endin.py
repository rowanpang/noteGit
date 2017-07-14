#!/usr/bin/python

import struct 
import binascii
import socket

hexStr = "3456"
hexStr = "53eb402c"
buf = binascii.unhexlify(hexStr)

print("for str %s,struct.unpack as:" %hexStr)
print(" <H: %s" %(hex(struct.unpack("<i",buf)[0])))
print(" >H: %s" %(hex(struct.unpack(">i",buf)[0])))
print("  H: %s" %(hex(struct.unpack("i",buf)[0])))
print("      ntohs: %s" %(hex(socket.ntohl(struct.unpack("i",buf)[0]))))
print(" !H: %s" %(hex(struct.unpack("!i",buf)[0])))
print("      ntohs: %s" %(hex(socket.ntohl(struct.unpack("!i",buf)[0]))))

hexStr = "53eb402c"
print(' IP: %s' %(socket.inet_ntoa(buf)))
