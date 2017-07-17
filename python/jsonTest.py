#!/usr/bin/env /usr/bin/python
#coding: utf-8

import json

repbody='{"DeviceID": "536774", "UserName": "pangweizhenbj"}'
repDict=json.loads(repbody)

print repDict
print type(repDict)
print repDict['DeviceID']

ret0={}
ret1={}
ret2={}
l = []                                                                              
ret0["a"] = "Aaaaaaaa"
ret0['b'] = 'Abbbbbbb'
ret0['c'] =  13333333
l.append(ret0)

ret1["a"] = "Baaaaaaa"
ret1['b'] = 'Bbbbbbbb'
ret1['c'] =  23333333
l.append(ret0)

print l

ret2['num'] = 2
ret2['info'] = l

print ret2

print json.dumps(ret2)
