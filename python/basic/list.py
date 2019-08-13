#!/usr/bin/python
# -*- coding: utf-8 -*-

print '--------list----------'
l = []
l.append(['a','b'])
l.append(('a','b'))
l.append('cd')
l.append(['e'])
l.append('f')
print l

str = 'a=aaaaaaa'
l.append(str.split('='))
l.extend(['g','h'])             #x.extend(xx)
print l

ll = list('abcd')
print ll
ll = list(['abcd'])
print ll
ll = list(('abcd','ef'))
print ll

lists = [[]] * 3                # []*3 pointers to a single empty list.
print lists
lists[0].append(3)
print lists                     #so change one of them will modify all.

lists = [ [] for i in range(3)]
print lists
lists[0].append(3)
lists[2].append(7)
print lists
