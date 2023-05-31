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
    a,./shmwrite 1111
        $ ipcs -m
            0x00000000 589968     root       600        5496832    2          dest
            0x0000004a 753817     root       777        131072     2
            0x7724f938 753819     root       666        32         0                        #这条记录
            0x00000000 688295     root       600        847872     2          dest
            0x00000000 688296     root       600        847872     2          dest
    b,./shmread


3,shm
    a,./shm
    b,
        $ pmap -X `pgrep shm`
        825440:   ./shm
             Address Perm   Offset Device   Inode Size  Rss Pss Referenced Anonymous LazyFree ShmemPmdMapped FilePmdMapped Shared_Hugetlb Private_Hugetlb Swap SwapPss Locked THPeligible Mapping
            00400000 r--p 00000000  00:22 3190682    4    4   4          4         0        0              0             0              0               0    0       0      0           0 shm
            .......
        7f9cfbb92000 rw-s 00000000  00:01  753817  128    4   2          4         0        0              0             0              0               0    0       0      0           0 SYSV0000004a (deleted)
        7f9cfbbb2000 rw-s 00000000  00:01  753817  128    4   2          4         0        0              0             0              0               0    0       0      0           0 SYSV0000004a (deleted)
        7f9cfbbd2000 rw-p 00000000  00:00       0    8    4   4          4         4        0              0             0              0               0    0       0      0           0
        ................
                                                  ==== ==== === ========== ========= ======== ============== ============= ============== =============== ==== ======= ====== ===========
                                                  2604 1480 179       1480        84        0              0             0              0               0    0       0      0           0 KB

    c,即便没有rm shmid(shmdt)之前 pmap 也是显示为 (deleted),而posix只在只在删除/dev/shm/a 文件后才标记为deleted. why??
        因为shmefs inode 对应的dentry没有dhash.
