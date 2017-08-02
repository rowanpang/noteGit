#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii
import re
import sys
import string

def strHexdump(strTmp):
    strlen = len(strTmp)
    perLinebits = 4        #2^4 = 16 char perline  
    perLine = 1 << perLinebits
    maxlen = (strlen + perLine - 1) >> perLinebits

    idxMaxbit = 0
    while (maxlen > 0):
        maxlen >>= 1
        idxMaxbit += 1
    idxMaxbit = (((idxMaxbit + 4 - 1) >> 2) + 1 ) & (~1)

    gapSize = 1
    idxFormat = '%0' + '%d' %(idxMaxbit) + 'x' + ' ' * gapSize

    # header line
    print(' ' * (idxMaxbit + gapSize)),
    for i in range(perLine):
        print ('%2x') %(i),
    print ''

    for idx in range(0,strlen,perLine):
        print idxFormat %(idx),
        outlen=0
        for j in range(perLine):
            if (j+idx) >= strlen:
                break
            outlen += 1
            print '%02x' %ord(strTmp[idx+j]),
            # print '%s' %(binascii.hexlify(strTmp[idx+j])),

        if outlen != perLine:
            for j in range(outlen,perLine):
                print ' '*2,                    #补全后面的空格.   

        print(' ' * gapSize),                   #gap between  hexdump and char.
        for j in range(outlen):
            c = strTmp[idx+j]
            if c in string.printable and c not in string.whitespace or c == ' ':
                sys.stdout.write(c)
            else:
                sys.stdout.write('.')

        print('')

if __name__ == '__main__':
    strHexdump('\x00' * ((1<<7)+5))
