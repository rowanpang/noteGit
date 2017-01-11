#!/usr/bin/python
#need install nodejs rpm

import subprocess
import os,sys
from subprocess import Popen,PIPE
import json

jsObj='{length: 31,"ID": \'35430\',"LastAuthID": parseInt(0) }'
jsScript='var input = ' + jsObj + '\n' +    \
        'var ret = JSON.stringify(input)' + '\n' + \
        'console.log(ret)' + '\n'

jsFile=open("./test.js","w+")
jsFile.write(jsScript)
jsFile.close()

subArgs=["node","./test.js"]
p=Popen(subArgs,stdout=PIPE,stderr=PIPE)
p.wait()
subRet=p.communicate()
print type(subRet)
print type(subRet[0])
print subRet[0]

subDict=json.loads(subRet[0])
print type(subDict)
print subDict["ID"]
