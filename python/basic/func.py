#!/usr/bin/python
# -*- coding: utf-8 -*-

def func(arg = None):
    if not arg:
        print 'no args'
    else:
        print 'args %s' %(arg)

func("xx")
func()      #if no 'arg = None' python will make a error

#右边是最内层,返回的是最内层的计算结果
xPlusN=lambda N:(lambda x:x+N)
xPlus2=xPlusN(2)
print xPlus2(2)
print xPlus2(1)

print
xMult2=(lambda N:(lambda x:x*N))(2)
print xMult2(2)
print xMult2(1)

print
print (lambda N:(lambda x:x+N))(2)(2)

class logger:
    def _log(self,x,msg):
        print "%s,%s" %(x,msg)

    def __init__(self):
        for l in range(5):
            setattr(self,'debug%s' %(l),
                (lambda obj, x: (lambda msg: obj._log(x,msg)))(self,l))


log = logger()

log.debug1('level1')
log.debug1('level1---')
log.debug0('00000')
log.debug2('level2')
log.debug2('level2---')
