#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii

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
    print('%2x') %(perLine - 1)

    for idx in range(0,strlen,perLine):
        print idxFormat %(idx >> perLinebits),
        outlen=0
        for j in range(perLine):
            if (j+idx) >= strlen:
                break
            outlen += 1
            print '%s' %(binascii.hexlify(strTmp[idx+j])),

        if outlen != perLine:
            for j in range(outlen,perLine):
                print ' '*2,                    #补全后面的空格.   

        print(' ' * gapSize),                   #gap between  hexdump and char.
        print '%s' %(strTmp[idx:idx+outlen])    #org char and new line

if __name__ == '__main__':
    strHexdump('3' * ((1<<5)+5))
