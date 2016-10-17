#!/usr/bin/python
#coding: utf-8

import sys

print '庞文字'

print len(sys.argv)
for arg in sys.argv:
	print arg

src1 = 1
src2 = 2
src3 = 3

# srcarg = 'src%s' %sys.argv[1]
# src = srcarg
# print src

src = 0

if len(sys.argv) >=2:
	print 'has arg'
	if sys.argv[1] == '1':
		print 'con 1'
		src = src1
	if sys.argv[1] == '2':
		print 'con 2'
		src = src2
	if sys.argv[1] == '3':
		print 'con 3'
		src = src3

print 'here %d' %src
