0,basic
    申请一定容量的内存,看需要多大的pgtable.

    stress -m 1 --vm-keep --vm-bytes 1G         or
    ./a.out 31 3

1, grep PageTable /proc/meminfo

