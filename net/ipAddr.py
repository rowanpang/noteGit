#!/usr/bin/python

import struct 
import binascii
import socket

# Source: 83.235.64.44

hexStr = "53eb402c"
buf = binascii.unhexlify(hexStr)
ipStr = socket.inet_ntoa(buf)
print(' IP: %s' %(ipStr))

ipHex = socket.inet_aton(ipStr)
print(' IP: %s' %(binascii.hexlify(ipHex)))


