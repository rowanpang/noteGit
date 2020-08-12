#!/usr/bin/python
import os
import sys
import commands

class thost():
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

def readcfg(thosts):
	fd = open('ssh.cfg','r')
	lines = fd.readlines()
	fd.close()
	for line in lines:
		if line.find('#') < 0:
			user = thost()
			dt = line.split(',')
			user.setIp(dt[0])
			user.setHost(dt[1])
			user.setPasswd(dt[2][:-1])
			user.setUser("root")
			thosts.append(user)

def makeNew():
    	print("make new expect\n");
	mknewcmd = "expect ./expmod/ssh_mknew.exp"
	os.system(mknewcmd)

def make():
	print("make new id_rsa\n");
	if not os.path.exists("/root/.ssh/id_rsa.pub"):
		makeNew()

def reset(deep):
	print("/root/.ssh/ file reset deep=%d" %deep)
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

def copyid(thosts):
	for host in thosts:
		cmd = "expect ./expmod/ssh_copyid.exp {0} {1} {2}".format(host.getIp(),host.getUser(),host.getPasswd())
		os.system(cmd)

def check(thosts):
	for host in thosts:
		cmd = "expect ./expmod/ssh_check.exp {0} {1} {2}".format(host.getIp(),host.getUser(),host.getPasswd())
		os.system(cmd)

def getknownhost(thosts):
	for host in thosts:
		cmd = "expect ./expmod/ssh_getknownhost.exp {0} {1} {2}".format(host.getHost(),host.getUser(),host.getPasswd())
		os.system(cmd)

		#cmd = "expect ./expmod/ssh_getknownhost.exp {0} {1} {2}".format(host.getIp(),host.getUser(),host.getPasswd())
		#os.system(cmd)

def clustertrust(thost):
	tmphosts = thost;
	tmphosts.pop(0)
	for host in tmphosts:
		cmd = "ssh {0} mktemp -d /tmp/tmp.sshauto.XXXXXXXX".format(host.getIp())
		ret,dt = commands.getstatusoutput(cmd)
		print("ssh %s ,tmpdir %s\n" %(host.getIp(),dt))
		
		cmd = "scp -r ./* {0}:{1}".format(host.getIp(),dt)
		os.system(cmd)
		
		cmd = "scp -r /etc/hosts {0}:/etc/hosts".format(host.getIp())
		os.system(cmd)

		cmd = 'ssh {0} "cd {1};python ./sshAuto.py mcopy"'.format(host.getIp(),dt)
		os.system(cmd)
		
def Usage():
	print "ssh_no_passwd  [resetdist|reset|make|copy|mcopy]"
	print "resetdist: clean .ssh dir"
	print "reset    : clean .ssh/id_rsa,id_rsa.pub"
	print "make     : make new .ssh/*"
	print "copy     : copy self's pass to another"
	print "khost    : get another host in known list"
	print "mcopy    : make and copy"
	print "check    : ssh auth check"


def main():
	if len(sys.argv) != 2:
		Usage()
		sys.exit(1)

	thosts = list()
	readcfg(thosts)

	if sys.argv[1] == "resetdist":
		reset("deep")
	elif sys.argv[1] == "reset":
		reset("")
	elif sys.argv[1] == "make":
		make()
	elif sys.argv[1] == "copy":
		copyid(thosts)
	elif sys.argv[1] == "khost":
		getknownhost(thosts)
	elif sys.argv[1] == "mcopy":
		make()
		copyid(thosts)
		getknownhost(thosts)
	elif sys.argv[1] == "check":
		check(thosts)
	elif sys.argv[1] == "cluster":
		clustertrust(thosts)
	else:
		Usage()


if __name__=="__main__":
	main()
