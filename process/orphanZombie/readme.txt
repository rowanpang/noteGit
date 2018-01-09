1,orphan
    执行./orphan
    父进程退出,子进程继续执行.因为没有close stdout,所以依然可以看到信息.
    orphan 的父进程变为init 进程.

2,zombie
    执行 ./zombie
	Im parent,cpid:4821
	Im child
    ps -elfH
	0 S root      4820 xxx	./zombie
	1 Z root      4821 xxx	    [zombie] <defunct>

    sleep 之后
	2个进程都退出,zombie进程被父进程回收.
