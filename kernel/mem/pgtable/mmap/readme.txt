0,basic
    a, mmap 情况下看pgtable 的占用量.
    b, 多个进程共享一段内存,每个进程需要自己的pgtable.

1,flow
    a, vmstat --wide 1  #观察内存变化
    b, 启动测试
        1), ./fbase 34       #创建fd 并写如数据
            #完成写入后,vmstat 内存信息基本稳定.
        2), ./fbase 34       #新开终端执行, 执行read.
            #read过程中vmstat 显示并没有新增加cache,输出基本稳定.
            #因为是mmap,所以与第一个进程共用物理内存.

    c, grep PageTable /proc/meminfo
        a, 第一个进程/第二个进程启动后 PageTable的占用量都会上涨.

