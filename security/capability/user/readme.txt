files:
	[root@rowanInspur user]# ll
	total 16
	-rw-rw-r--. 1 pangwz pangwz  829 Jan 28 18:01 main.c
	-rw-rw-r--. 1 pangwz pangwz  447 Jan 28 18:18 Makefile
	-rw-rw-r--. 1 pangwz pangwz 1105 Jan 28 18:17 readme.txt
	----------. 1 root   root     45 Jan 28 10:02 test.txt
	[root@rowanInspur user]#

prepare:
	使用normal用户执行如下命令
	make
	make run

log分析:
	------------------
	./cap_cap_a
	try read file 1
	read:This is line 1,len:15
	uid:500,euid:500
	after setuid(74)uid:74,euid:74
	try read file 2
	read:This is line 1,len:15
#-------->设置了两个cap所以，可以实现读文件即setuid操作.
	------------------
	./cap_a
	try read file 1
	read:This is line 1,len:15
	uid:500,euid:500
	when setuid(74): Operation not permitted
#-------->仅仅设置了一个cap所以可以实现读操作，但是不能setuid
	------------------
	./suid_a
	try read file 1
	read:This is line 1,len:15
	uid:500,euid:0
	after setuid(74)uid:74,euid:74
	try read file 2
	when open test.txt: Permission denied
	make: *** [run] Error 1
#-------->设置了suid，运行时为root权限即euid，所以可以读文件及setuid操作，
#-------->但是setuid之后的第二次读文件因为euid改变而失败
