#!/usr/bin/python

import aes
import binascii

key = 'a'*16
key = b'\xe5]\x98\xf4\xb9t\xf4h\x96^\xea\x9d\xb0\xe8\xc0\xb1'
data = binascii.unhexlify('9c9ddd')
data = binascii.unhexlify('1400000c5b513fced1aece9189bc69c2')
print binascii.hexlify(data)
dataAES = aes.encryptData(key,data)
print binascii.hexlify(dataAES)
dataDeAES = aes.decryptData(key, dataAES)
print binascii.hexlify(dataDeAES)
