#!/bin/python
# -*- coding: utf-8 -*-


#python中 arg,*args,**kwargs,当然这几个变量名换成其他无所谓，主要是(没有*)变量名，*变量名，**变量名。 这几个相对顺序不能变
#arg表示某个参数
def func1(arg):  #一个变量
    print(arg)
func1(1)

def func2(*args):  #可变参数列表,*args 用来将参数打包成tuple给函数体调用,输出结果以元组的形式展示
    print(args,type(args))
func2(1,2,3)

def func3(arg1,arg2,*args):
    print('arg1:',arg1)
    print('arg2:',arg2)
    print('args:',args)
func3(1,2,3,4,5,6,7)

def func4(**kwargs):   # **kwargs 打包关键字参数成dict给函数体调用,输出结果以dict形式展示
    print(kwargs,type(kwargs))
func4(a=1,b=2,c=3,d=4)

def func5(arg,*args,**kwargs):
    print(arg,args,kwargs)
func5(1,2,3,4,a=5,b=6,c=7)
