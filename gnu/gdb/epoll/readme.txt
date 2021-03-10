0,basic
    a,


1,x64
    a,gdb epoll/simple 程序都可以正常工作, epoll 没有INTER

2,ppc64le
    a,gdb simple    ok
    b,gdb epoll     ng
        ret=-1,errno == EINTR(4)

