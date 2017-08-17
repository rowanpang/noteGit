#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import os.path
import re

rootdir =  '/home/pangwz/Downloads'                                 

# for parent,dirnames,filenames in os.walk(rootdir):  
    # for dirname in  dirnames:                      
        # print "parent is:" + parent
        # print  "dirname is" + dirname
    # for filename in filenames:                    
        # print "parent is:" + parent
        # print "filename is:" + filename
        # print "the full name of the file is:" + os.path.join(parent,filename) 

#len('我') = 3,utf8中文为3个字节编码.
def findChineses(u8str):
    if isinstance(u8str, unicode):
        return ''
    txt = u8str.decode('utf8')
    res = re.findall(u"[\u4e00-\u9fa5]", txt)   # \u4e00-\u9fa5是中文常用字段
    return res

#ftl,全角字符总数(org + pad).
#fpad, 全角填充字符.
#htl,xxx
#fpad,xxx
def myAlign(un_align_str, htl=0, ftl=0, hpad=' ', fpad='　'):
    assert isinstance(htl, int)                # 输入长度是否为整数，否则报错
    assert isinstance(ftl, int)                # 输入长度是否为整数
    if (htl+ftl*2) <= len(un_align_str):      # 小于输入长度返回原字符
        return un_align_str
    strlen = len(un_align_str)
    chn = findChineses(un_align_str)
    numchn = len(chn)
    numsph = strlen - numchn * 3
    # print (htl-numsph)
    # print (ftl-numchn)
    str = hpad*(htl-numsph) + fpad*(ftl-numchn)

    # # 以下仅供测试输出
    # print '\n' + '='*20
    # print 'str = ', un_align_str
    # for i in range(numchn):
        # print 'Chinese = ', chn[i]
    # print '中文字符数 = ', numchn
    # print '半角字符数 = ', numsph
    # print '全角字符数 = ', numchn
    # print '总字符数 = ', strlen
    return str

for f in os.listdir(rootdir):
    print '%s is dir: %s' %((f+myAlign(f,22,10)), True if os.path.isdir('%s/%s' %(rootdir,f)) else False)
