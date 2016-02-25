#!/usr/bin/python

import socket
import os
import select
import time
import subprocess
ADDR="./sk_server"
BUFFSIZE=2048

if __name__ == '__main__':
	print "-------server start----"
	sr_sk=socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
	try:
		os.unlink(ADDR) 
	except OSError,e:
		print e.errno
		if e.errno == os.errno.ENOENT:
			print 'equal'
		else:
			os.exit(e.errno)
	sr_sk.bind(ADDR)
	sr_sk.listen(8)
	while 1:
		print "----new loop------"
		select.select([sr_sk],[],[])	
		print "after select-------"
		conn_cl,addr_cl=sr_sk.accept()
		print "after accept-------"
		cmd_cl=conn_cl.recv(BUFFSIZE)
		print cmd_cl 
		#cmd_ret = os.popen(cmd_cl);
		cmd_ret = subprocess.Popen(cmd_cl,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
		cmd_ret.poll()
		sr_ret=cmd_ret.stdout.read()
		conn_cl.send(sr_ret)
		#time.sleep(10)

	sr_sk.close()
