#!/usr/bin/env /usr/bin/python
#coding: utf-8

import json

repbody='{"DeviceID": "536774", "UserName": "pangweizhenbj"}'
repDict=json.loads(repbody)

print repDict
print type(repDict)
print repDict['DeviceID']


dataGetInfo= {
                    'services':
                        [
                            {'id': 1,'name':'http','enable':1},
                            {'id': 2,'name':'http2','enable':11},
                        ]
                        ,
                    'ports':
                        [
                            {'id': 1,'name':'port','enable':1},
                            {'id': 2,'name':'port','enable':11},
                        ]
                    }
dj = json.dumps(dataGetInfo)
print dj
print type(dj)
d = json.loads(dj)
print d
print type(d)
print type(d['services'])
