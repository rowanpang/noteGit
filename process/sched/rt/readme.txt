0,basic
    a,ref
	http://blog.chinaunix.net/uid-24774106-id-3379478.html

1,fifo
    a,终端1
	stap cxswitch.stp 70
    b,终端2
	./psinfo.sh
    c,终端3
	./comp.sh 1

2,RR
    终端1:
	stap cxswitch.stp 70
    终端2:
	./psinfo.sh
    终端3:
	./comp.sh 2


