#!/bin/python
# -*- coding: utf-8 -*-	

table=u'''乘客姓名,性别,出生日期
HuangTianhui,男,1948/05/28
姜翠云,女,1952/03/27
李红晶,女,1994/12/09
LuiChing,女,1969/08/02
宋飞飞,男,1982/03/01
唐旭东,男,1983/08/03
YangJiabao,女,1988/08/25
提江·阿布拉,男,1979/07/10
安文兰,女,1949/10/20
胡偲婠(婴儿),女,2011/02/25
(有待确定姓名),男,1985/07/20
'''
# print type(table)
print table.encode('utf8')
# 因为显示效果是一个中文占2个英文字符. 而len('庞'.gb18030) == 2,所以转为gb18030
table = table.encode('gb18030') 
# print type(table)
data = [[cell  for cell in row.split(b",")] for row in table.split(b"\n")  if row]
# print data
len0 = max([len(x[0]) for x in data])
len1 = max([len(x[1]) for x in data])
len2 = max([len(x[2]) for x in data])

for cell in data:
    print("_" * (len0+len1+len2+4))
    line = (
        b"|" + cell[0].rjust(len0) +
        b"|" + cell[1].rjust(len1) +
        b"|" + cell[2].rjust(len2) + b"|"
    )
    print(line.decode('gb18030').encode('utf8'))
