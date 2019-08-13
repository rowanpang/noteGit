#!/usr/bin/python
# -*- coding: utf-8 -*-
# sort 比较的是obj.inter()得到的list.
# 所以，对collections sorted 的返回结果为key ref ./collections.py
L = [('b',3),('a',4),('c',2),('d',1)]
print type(L)
for l in L:
    print l
    print type(l)

print "key:-----"
print sorted(L,key=lambda x:x[0])
print sorted(L,key=lambda x:x[1])

#L = ['a','c','b','d']
# print sorted(L,cmp=lambda x,y:cmp(x[1],y[1]))   #error

print "cmp:-----"
print sorted(L,cmp=lambda x,y:cmp(x[0],y[0]))   #error
print sorted(L,cmp=lambda x,y:cmp(x[1],y[1]))   #error
