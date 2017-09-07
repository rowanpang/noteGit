#!/usr/bin/python

import socket
import os
import select
import client

if __name__ == '__main__':
	srr_ack=client.client_exec_cmd("/sbin/chkconfig --list")	
	print srr_ack
