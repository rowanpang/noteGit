#!/usr/bin/python

import struct 
import binascii
import socket

buf = binascii.unhexlify("3456")

print("for str '3456',struct.unpack as:")
print(" <H: %s" %(hex(struct.unpack("<H",buf)[0])))
print(" >H: %s" %(hex(struct.unpack(">H",buf)[0])))
print("  H: %s" %(hex(struct.unpack("H",buf)[0])))
print("      ntohs: %s" %(hex(socket.ntohs(struct.unpack("H",buf)[0]))))
print(" !H: %s" %(hex(struct.unpack("!H",buf)[0])))
print("      ntohs: %s" %(hex(socket.ntohs(struct.unpack("!H",buf)[0]))))
