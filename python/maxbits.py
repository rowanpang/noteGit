#!/usr/bin/python
# -*- coding: utf-8 -*-

i = 1<< 17 | 1 << 16 | (1 << 4) + 145
print 'test number: %x' %i
j = 0       # max bit num
k = 0       # number of bits setted
while (i > 0):
    j += 1
    if (i & 1):
        k += 1
    i >>= 1 

print j
print k
