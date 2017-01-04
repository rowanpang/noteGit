#!/usr/bin/python

def func(arg = None):
    if not arg:
        print 'no args'
    else:
        print 'args %s' %(arg)

func("xx")
func()      #if no 'arg = None' python will make a error
