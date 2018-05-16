#!/usr/bin/python
# -*- coding: utf-8 -*-

import openpyxl

wb = openpyxl.Workbook()
ws = wb.active

title = ["工号","部门","姓名","4.29","4.30","5.1","法定","法定其它"]
ws.append(title)

for l in open("./new.txt").readlines():
    if l.strip():
        pass

wb.save('new.xlsx')
