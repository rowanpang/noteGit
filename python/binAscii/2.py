#!/usr/bin/python2

import binascii
print binascii.unhexlify('abac')            #abac as hex str \xab\xac
print binascii.unhexlify('9c9d')

print binascii.hexlify('26')                #str 15 convert to 3135
