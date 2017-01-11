#!/usr/bin/env /usr/bin/python
#coding: utf-8

import json

repbody='{"DeviceID": "536774", "UserName": "pangweizhenbj"}'
repDict=json.loads(repbody)

print repDict
print type(repDict)
print repDict['DeviceID']



