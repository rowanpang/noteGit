#!/usr/bin/python
import struct

for i in open('in.txt').read().replace('\n', '').split(':'):
	open('ggggg', 'ab').write(struct.pack('B',eval('0x%s'% i)))
