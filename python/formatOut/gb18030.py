#!/bin/python
# -*- coding: gb18030-*-	

#!!!!!readme:!!!!!
    #vim 编辑器需要将这个文件的编码设置为gb18030
    # set fileenconding
    # fileencoding = gb18030

import binascii

strl = ['庞伟振s','s'*10]

for s in strl:
    print binascii.hexlify(s)
    print '%20s' %(s)
