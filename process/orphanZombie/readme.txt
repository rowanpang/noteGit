1,orphan
    a,执行./orphan
	父进程退出,子进程继续执行.因为
	没有close stdout,所以依然可以看到信息.
	orphan 的父进程变为init 进程.

    b,pid,pgid,sid 的变换
	1),parent 未退出
	     4203  4203  4203     -zsh
	    21583 21583  4203       ./orphan
	    21584 21583  4203         ./orphan
	2),orphaned
	    21584 21583  4203   ./orphan

	pid,pgid,sid 不改变.
2,zombie
    执行 ./zombie
	Im parent,cpid:4821
	Im child
    ps -elfH
	0 S root      4820 xxx	./zombie
	1 Z root      4821 xxx	    [zombie] <defunct>

    sleep 之后
	2个进程都退出,zombie进程被父进程回收.

3,
    ref forkMulti*.txt
