#!/usr/bin/python2
import sys
import time
import datetime

#default buffer IO stdout, same as glibc
print ''
print ''
print '%s' % datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

print "--------- flush none----"
print "--------- flush false---"
print "--------- flush true----"

time.sleep(5)
