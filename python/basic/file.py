#!/usr/bin/python
import os,sys

fobj=open("/tmp/test","w+")
fobj.write("asdfjkj")
fobj.close()

print type(fobj)


fpath="/tmp/file2"
if os.path.exists(fpath):
    print "fpath: %s is exist" % fpath

if os.path.isfile(fpath):
    print "fpath: %s is a normal file" % fpath
else:
    print "fpath: %s is not exist or not a normal file" % fpath

fpath="/tmp/file1"
if os.access(fpath,os.F_OK):                    #if file1 is a dir also return true
    print "fpath: %s is exist" % fpath
else:
    print "fpath: %s is not exist" % fpath

if os.access(fpath,os.R_OK):                    #if file1 is a dir also return true
    print "fpath: %s is able to read" % fpath
else:
    print "fpath: %s is not able to read" % fpath

if os.access(fpath,os.X_OK):                    #if file1 is a dir also return true
    print "fpath: %s is able to execute" % fpath
else:
    print "fpath: %s is not able to execute" % fpath
