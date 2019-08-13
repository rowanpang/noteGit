#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii

strU8= '庞伟振'
print('type of strU8: %s' %(type(strU8)))
strU8_decode = strU8.decode('utf8')
print('type of strU8_decode: %s' %(type(strU8_decode)))

strUnicode = u'庞伟振'
print('type of strUnicode: %s' %(type(strUnicode)))
strUnicode_encode = strUnicode.encode('utf8')
print('type of strUnicode_encode: %s' %(type(strUnicode_encode)))

print('hexlify(strUnicode):%s' %(binascii.hexlify(strUnicode.encode('utf8'))))

strUnicode_encode = strUnicode.encode('utf32')
print('type of strUnicode_encode: %s' %(type(strUnicode_encode)))

print len(u'a')
print len(u'庞')
print len('庞')
strU8='庞'
print '庞-gb18030',len(strU8.decode('utf8').encode('gb18030'))
print binascii.hexlify(strU8.decode('utf8').encode('gb18030'))

strU8='a'
print 'a-gb18030',len(strU8.decode('utf8').encode('gb18030'))
print binascii.hexlify(strU8.decode('utf8').encode('gb18030'))
