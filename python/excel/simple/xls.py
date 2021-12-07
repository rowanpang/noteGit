#!/usr/bin/python

import xlrd

wb = xlrd.open_workbook('./dayRecord.xls')
for sheet in wb.sheet_names():
    shn = excelobj.sheet_by_name(sheet)
    print shn
