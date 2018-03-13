#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import os.path
import re

rootdir =  '/home/pangwz/Downloads'
rootdir =  '/root/Downloads'

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
    assert isinstance(htl, int)                 # 输入长度是否为整数，否则报错
    assert isinstance(ftl, int)                 # 输入长度是否为整数
    if (htl+ftl*2) <= len(un_align_str):        # 小于输入长度返回原字符
        return un_align_str
    strlen = len(un_align_str)
    chn = findChineses(un_align_str)
    numchn = len(chn)
    numsph = strlen - numchn * 3                #按照utf8处理时len('庞'.utf8)==3,所以为3
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

def myAlign2(un_align_str, length=0, addin=' '):
    assert isinstance(length, int)        # 输入长度是否为整数
    if length <= len(un_align_str):       # 小于输入长度返回原字符
        return un_align_str
    strlen = len(un_align_str)
    chn = findChineses(un_align_str)
    numchn = len(chn)
    numsp = length - strlen + numchn      # 填充半角字符的的个数
        #+numchn,是补齐一个utf8 中文字符比起len()少占了一个.
        #len('庞'.utf8)==3,但是输出只占2位.
    str = addin * numsp                   # 生成填充字符串

    return str                            # 返回填充的字符串

for f in os.listdir(rootdir):
    # print '%s is dir: %s' %((f+myAlign(f,22,10)), True if os.path.isdir('%s/%s' %(rootdir,f)) else False)
    print '%s is dir: %s' %((myAlign2(f,24)), os.path.isdir('%s/%s' %(rootdir,f)))
    #这里的’23‘在输出时按照pad = 23 - len(str) 计算,
    #因为utf8下len('庞')==3,但是输出只占2,所以出现对齐问题.
    # print '%-23s is dir: %s' %((f), os.path.isdir('%s/%s' %(rootdir,f)))
