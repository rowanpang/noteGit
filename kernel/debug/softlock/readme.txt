1,
    在内核态不释放cpu delay 30s,让内核报softlock up error.
    与 /proc/sys/kernel/watchdog_thresh 的值有关，delay时间最好>=3*thresh
