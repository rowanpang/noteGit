0,basic
    a,userspace UAF
        free 一个struct 后,还引用这个struct的成员(一般是指针类型成员)
        内核态UAF也是,free struct 后,使用struct的指针类型成员.

$ ./simple                                                                       (master↑1|●2✚5…1)
    ------just after zalloc pre valued------
    size:0,strpredef:(nil),stralloc:(nil)
    rstr->strpredef:(null)                                                      #printf 自动过滤掉了null str, 不去引用. 不会coredump
    rstr->stralloc:(null)
    ------member valued------
    size:10,strpredef:0x402010,stralloc:0xab86d0
    rstr->strpredef:Hello!
    rstr->stralloc:Hello!
    -------After free(rstr)-------
    size:10,strpredef:0xa,stralloc:0xab8010                                     #free之后,地址类成员,值已经异常.
    ----before UAF -------
    [1]    1722339 segmentation fault (core dumped)  ./simple                   #printf 通过这个地址取str, 因为地址异常coredump.
