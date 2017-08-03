#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii

strFileCoding= '庞伟振'
print('type of strFileCoding: %s' %(type(strFileCoding)))
strFileCoding_decode = strFileCoding.decode('utf8')
print('type of strFileCoding_decode: %s' %(type(strFileCoding_decode)))

strUnicode = u'庞伟振'
print('type of strUnicode: %s' %(type(strUnicode)))
strUnicode_encode = strUnicode.encode('utf8')
print('type of strUnicode_encode: %s' %(type(strUnicode_encode)))

print('hexlify(strUnicode):%s' %(binascii.hexlify(strUnicode.encode('utf8'))))

strUnicode_encode = strUnicode.encode('utf32')
print('type of strUnicode_encode: %s' %(type(strUnicode_encode)))
