#!/usr/bin/python
import os
import sys
import commands

class USER():
	def __init__(self):
		self.ip = ""
		self.passwd = ""
		self.user = ""
		self.host = ""
	def setHost(self,host):
		self.host = host
	def getHost(self):
		return self.host
	def setIp(self,ip):
		self.ip = ip
	def setPasswd(self,passwd):
		self.passwd = passwd
	def setUser(self,user):
		self.user = user
	def getIp(self):
		return self.ip
	def getPasswd(self):
		return self.passwd
	def getUser(self):
		return self.user

def readcfg(userlst):
	fd = open('ssh.cfg','r')
	lines = fd.readlines()
	fd.close()
	for line in lines:
		if line.find('#') < 0:
			user = USER()
			dt = line.split(',')
			user.setIp(dt[0])
			user.setHost(dt[1])
			user.setPasswd(dt[2][:-1])
			user.setUser("root")
			userlst.append(user)

def make():
	mknewcmd = "expect ssh_mknew.exp"
	os.system(mknewcmd)

def reset(deep):
	if deep == "deep":
		os.system("rm /root/.ssh -rf")
	else:
		os.system("rm /root/.ssh/id_rsa")
		os.system("rm /root/.ssh/id_rsa.pub")

def _checkinknown(data):
	result = False
	cmd = "cat /root/.ssh/known_hosts| grep {0} | wc -l".format(data);
	ret,dt = commands.getstatusoutput(cmd)
	if not ret:
		if dt.isdigit() and int(dt) >= 1:
			result = True
	return result

def copyid(userlst):
	for ruser in userlst:
		cmd = "expect ssh_copyid.exp {0} {1} {2}".format(ruser.getIp(),ruser.getUser(),ruser.getPasswd())
		os.system(cmd)

def hostkeyAuth(userlst):
	for ruser in userlst:
		cmd = "expect ssh_hostkey.exp {0} {1} {2}".format(ruser.getIp(),ruser.getUser(),ruser.getPasswd())
		os.system(cmd)

def getknown(userlst):
	for ruser in userlst:
		if _checkinknown(ruser.getIp()):
			cmd = "expect ssh_copyid_noyes.exp {0} {1} {2}".format(ruser.getIp(),ruser.getUser(),ruser.getPasswd())
			os.system(cmd)
		else:
			cmd = "expect ssh_getknown.exp {0} {1} {2}".format(ruser.getHost(),ruser.getUser(),ruser.getPasswd())
			os.system(cmd)

		if _checkinknown(ruser.getHost()):
			cmd = "expect ssh_copyid_noyes.exp {0} {1} {2}".format(ruser.getHost(),ruser.getUser(),ruser.getPasswd())
			os.system(cmd)
		else:
			cmd = "expect ssh_getknown.exp {0} {1} {2}".format(ruser.getHost(),ruser.getUser(),ruser.getPasswd())
			os.system(cmd)

def Usage():
	print "ssh_no_passwd  [resetdist|reset|make|copy|mcopy]"
	print "resetdist: clean .ssh dir"
	print "reset    : clean .ssh/id_rsa,id_rsa.pub"
	print "make     : make new .ssh"
	print "copy     : copy self's pass to another"
	print "get      : get another in known list"
	print "mcopy    : make and copy"
	print "hostkey  : just say yes for host key"


def main():
	if len(sys.argv) != 2:
		Usage()
		sys.exit(1)

	userlst = list()
	readcfg(userlst)

	if sys.argv[1] == "resetdist":
		reset("deep")
	elif sys.argv[1] == "reset":
		reset("")
	elif sys.argv[1] == "make":
		make()
	elif sys.argv[1] == "copy":
		copyid(userlst)
	elif sys.argv[1] == "get":
		getknown(userlst)
	elif sys.argv[1] == "mcopy":
		make()
		copyid(userlst)
		getknown(userlst)
	elif sys.argv[1] == "hostkey":
		hostkeyAuth(userlst)
	else:
		Usage()


if __name__=="__main__":
	main()
