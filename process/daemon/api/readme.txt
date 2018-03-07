1,父进程退出后，子进程的父进程自动变为pid 1.
2,journalctl SYSLOG_IDENTIFIER=firstdaemon --follow
3,daemon()
    a,执行fork，如果成功 parent 执行exit()
    b,daemon 之后就是在child 进程中.
    c,man 3 daemon
	detach themselves from the controlling terminal and
	run in the background as system daemons


4,doWhat?
    (1),摆脱原会话的控制。该进程变成新会话期的首进程
    (2),摆脱原进程组。成为一个新进程组的组长
    (3),摆脱终端控制。如果在调用 setsid() 前，该进程有控制终端，那么与该终端的联系被解除
