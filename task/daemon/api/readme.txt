1,父进程退出后，子进程的父进程自动变为pid 1.
2,journalctl SYSLOG_IDENTIFIER=firstdaemon --follow
3,daemon()
    a,执行fork，如果成功 parent 执行exit()
    b,daemon 之后就是在child 进程中.
    c,man 3 daemon
	detach themselves from the controlling terminal and 
	run in the background as system daemons
