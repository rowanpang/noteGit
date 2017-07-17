#!/usr/bin/python
#coding:utf-8

import re

# 罗马数字,按照每一位的可能组合匹配.
    # I = 1
    # V = 5
    # X = 10
    # L = 50
    # C = 100
    # D = 500
    # M = 1000
    # 最多可重复3此.  MMM=3000 “成立于 MDCCCLXXXVIII”而不是“成立于1888”
pattern = '^M?M?M?(CM|CD|D?C?C?C?)(XC|XL|L?X?X?X?)(IX|IV|V?I?I?I?)$'
pattern = '^(M{0,3})(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$'
print re.search(pattern, 'MDLV').groups()          #1555

pattern = """
    ^                   #the beginning of the string
    (M{0,3})              #thought
    (CM|CD|D?C{0,3})
    (XC|XL|L?X{0,3})
    (IX|IV|V?I{0,3})
    $                   #the end of the string
    """
print re.search(pattern, 'MMDLV',re.VERBOSE).groups()          #1555

# phone number
phonePattern = re.compile(r'''
                # don't match beginning of string, number can start anywhere
    (\d{3})     # area code is 3 digits (e.g. '800')
    \D*         # optional separator is any number of non-digits
    (\d{3})     # trunk is 3 digits (e.g. '555')
    \D*         # optional separator
    (\d{4})     # rest of number is 4 digits (e.g. '1212')
    \D*         # optional separator
    (\d*)       # extension is optional and can be any number of digits
    $           # end of string
    ''', re.VERBOSE)

print phonePattern.search("8885551234").groups()
print phonePattern.search("8885551234").group(2)
print phonePattern.search("8885551234").group(2,4)
print phonePattern.search("8885551234").group(1,2,3,4)
print phonePattern.search("1-888-555-1234-4567").groups()

