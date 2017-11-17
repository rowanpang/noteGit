#!/usr/bin/python
# sort 比较的是obj.inter()得到的list.
# 所以，对collections sorted 的返回结果为key ref ./collections.py
L = [('b',4),('a',3),('c',1),('d',2)]
print type(L)
for l in L:
    print l
    print type(l)
print sorted(L,key=lambda x:x[0])
print sorted(L,key=lambda x:x[1])

L = ['a','c','b','d']
# print sorted(L,cmp=lambda x,y:cmp(x[1],y[1]))   #error
print sorted(L,cmp=lambda x,y:cmp(x[0],y[0]))   #error
