#!/usr/bin/python
class twice:
    def __init__(self):
        self.data = 0

    def add(self, x):
         self.data += x

    def addtwice(self, x):
         self.add(x)
         self.add(x)

print '-------------class twice-----------------'
b = twice()
print b.data
b.addtwice(5)
print b.data


print '\n\n'

class ctest():
    i = '23456'
    def func(self):
        return 'hello world'

print '-------------class ctest-----------------'
print ctest.i

ct = ctest()
print ct.i

ct.i = '45677'
print ctest.i
print ct.i

print '\n\n'

print '-------------class Dog-----------------'
class Dog:
    def __init__(self, name):
        self.name = name
        self.tricks = []    # creates a new empty list for each dog

    def add_trick(self, trick):
        self.tricks.append(trick)


f = Dog('ffff')
e = Dog('eeee')

f.add_trick('rooooo')
e.add_trick('eeeeeee')

print f.tricks
print e.tricks
