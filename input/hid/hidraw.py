#!/usr/bin/env python
#./hidraw /dev/hidrawX 
import sys
import struct
import binascii
from time import sleep

def main():
	if len(sys.argv) < 2:
		print "arg error"
		return
	i = 0
	hidraw = open(sys.argv[1],'r')
	while True:
		print "new read %d"  %i
		if i == 0:
			sleep(1)
		data = hidraw.read(8)
		# print ''.join(map(lambda x: (hex(ord(x))[2:] + ' ')[-3:], struct.unpack('!8c', data)))
		print binascii.b2a_hex(data)
		# hidraw.seek(0)
		i = (i + 1) % 70

if __name__ == '__main__':
	main()
