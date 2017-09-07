#!/usr/bin/python

import socket
import os
import select

ADDR="./sk_server"

def client_exec_cmd(strCmd):
	print "-------client init----"
	cl_sk=socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
	cl_sk.connect(ADDR)
	print "after connect-------"
	#cl_sk.send("send--from client")
	cl_sk.send(strCmd)
	sr_ack=cl_sk.recv(4096)
	#print sr_ack
	cl_sk.close()
	return sr_ack;

if __name__ == '__main__':
	srr_ack=client_exec_cmd("/sbin/chkconfig --list")	
	print srr_ack
