#!/usr/bin/python
import os,sys 

fobj=open("/tmp/test","w+")
fobj.write("asdfjkj")
fobj.close()

print type(fobj)
