1,basic
    a,shmwrite
        创建共享内存,并映射到进程空间.
        写入数据.
    b,shmread
        1),将共享内存映射到内存空间，
        2),读取数据
        3),并删除共享内存.

    b,shm
        一个共享内存可以在进程中映射多次

2,flow
    ./shmwrite 1111
    ./shmread
