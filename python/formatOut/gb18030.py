#!/bin/python
# -*- coding: gb18030-*-	

#!!!!!readme:!!!!!
    #vim �༭����Ҫ������ļ��ı�������Ϊgb18030
    # set fileenconding
    # fileencoding = gb18030

import binascii

strl = ['��ΰ��s','s'*10]

for s in strl:
    print binascii.hexlify(s)
    print '%20s' %(s)
