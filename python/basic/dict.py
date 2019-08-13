#!/usr/bin/python
# -*- coding: utf-8 -*-
print '--------dict----------'
a = ('abc', 'lalalla')
print dict([a])
print dict([['aaa','aaaaaaaa']])

a = [ 'abc=lalalla', 'appa=kdkdkdkd', 'kkakaka=oeoeoeo']
print [s.split('=') for s in a]
for s in a:
    l=[]
    # print s.split('=')
    l.append(s.split('='))
    print dict(l)

da = {"a":"aaaaaa",'b':'bbbbbbbb'}
print da
for d in da.keys():
    print d,da[d]


tmp={}
tmp["key1"]="key1"
tmp["key2"]="key2"
print tmp
