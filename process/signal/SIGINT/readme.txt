1,fork.c
    a,SIGINT, interrupt signal send by Ctrl+c
    b,fork 之后两个task都是前台进程,ctrl+c 将分别向两个task发送SIGINT
    c,2个task同时退出.

2,justSleep.c
    a,wakeup it and process SIGINT
    b,kill -INT `pid`

3,pthread.c
    a,ctrl + c,唤醒并退出所有的thread.
