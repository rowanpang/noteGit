#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
import requests

# r = requests.get("http://htime.inspur.com/l/getUserInfoByKey.ashx?Value=a")
r = requests.get('https://github.com/timeline.json')
print r.encoding
print r.headers
print r.content
print json.loads(r.content,encoding=r.encoding)

A=0
for A in map(chr,range(ord('a'),ord('z')+1)):
    print A
