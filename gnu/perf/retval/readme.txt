$ perf probe /root/noteGit/gnu/perf/dynso/libmsg.so mymsg%return %ax
    Added new event:
      probe_libmsg:mymsg   (on mymsg%return in /root/noteGit/gnu/perf/dynso/libmsg.so with %ax)

    You can now use it in all perf tools, such as:

	    perf record -e probe_libmsg:mymsg -aR sleep 1

$ echo 1 > events/probe_libmsg/mymsg/enable
$ echo 1 > tracing_on

$ cat events/probe_libmsg/mymsg/format
    name: mymsg
    ID: 1640
    format:
	    field:unsigned short common_type;	offset:0;	size:2;	signed:0;
	    field:unsigned char common_flags;	offset:2;	size:1;	signed:0;
	    field:unsigned char common_preempt_count;	offset:3;	size:1;	signed:0;
	    field:int common_pid;	offset:4;	size:4;	signed:1;

	    field:unsigned long __probe_func;	offset:8;	size:8;	signed:0;
	    field:unsigned long __probe_ret_ip;	offset:16;	size:8;	signed:0;
	    field:u64 arg1;	offset:24;	size:8;	signed:0;

    print fmt: "(%lx <- %lx) arg1=0x%Lx", REC->__probe_func, REC->__probe_ret_ip, REC->arg1

$ cat trace
    # tracer: nop
    #
    # entries-in-buffer/entries-written: 11/11   #P:4
    #
    #                              _-----=> irqs-off
    #                             / _----=> need-resched
    #                            | / _---=> hardirq/softirq
    #                            || / _--=> preempt-depth
    #                            ||| /     delay
    #           TASK-PID   CPU#  ||||    TIMESTAMP  FUNCTION
    #              | |       |   ||||       |         |
	   dynso-23031 [002] d... 4419166.112818: mymsg: (0x4007cf <- 0x7f063360360a) arg1=0x59f7 <------------
                                                                                                               \
$ make;./dynso                                                                          (master|…)             |
    cmd:cat /proc/23031/maps, pid:23031,0x59f7   #返回值与pid对应.--------------------------------------------/
    00400000-00401000 r-xp 00000000 fd:00 6950773                            /root/noteGit/gnu/perf/dynso/dynso
    00600000-00601000 r--p 00000000 fd:00 6950773                            /root/noteGit/gnu/perf/dynso/dynso
    00601000-00602000 rw-p 00001000 fd:00 6950773                            /root/noteGit/gnu/perf/dynso/dynso
    00f8a000-00fab000 rw-p 00000000 00:00 0                                  [heap]
    7f063322e000-7f06333f9000 r-xp 00000000 fd:00 10756900                   /usr/lib64/libc-2.25.so
    7f06333f9000-7f06335f9000 ---p 001cb000 fd:00 10756900                   /usr/lib64/libc-2.25.so
    7f06335f9000-7f06335fd000 r--p 001cb000 fd:00 10756900                   /usr/lib64/libc-2.25.so
    7f06335fd000-7f06335ff000 rw-p 001cf000 fd:00 10756900                   /usr/lib64/libc-2.25.so
    7f06335ff000-7f0633603000 rw-p 00000000 00:00 0
    7f0633603000-7f0633604000 r-xp 00000000 fd:00 6950774                    /root/noteGit/gnu/perf/dynso/libmsg.so
    7f0633604000-7f0633803000 ---p 00001000 fd:00 6950774                    /root/noteGit/gnu/perf/dynso/libmsg.so
    7f0633803000-7f0633804000 r--p 00000000 fd:00 6950774                    /root/noteGit/gnu/perf/dynso/libmsg.so
    7f0633804000-7f0633805000 rw-p 00001000 fd:00 6950774                    /root/noteGit/gnu/perf/dynso/libmsg.so
    7f0633805000-7f063382b000 r-xp 00000000 fd:00 10758760                   /usr/lib64/ld-2.25.so
    7f06339f7000-7f06339fa000 rw-p 00000000 00:00 0
    7f0633a28000-7f0633a2a000 rw-p 00000000 00:00 0
    7f0633a2a000-7f0633a2b000 r--p 00025000 fd:00 10758760                   /usr/lib64/ld-2.25.so
    7f0633a2b000-7f0633a2d000 rw-p 00026000 fd:00 10758760                   /usr/lib64/ld-2.25.so
    7ffdc77ba000-7ffdc77db000 rw-p 00000000 00:00 0                          [stack]
    7ffdc77df000-7ffdc77e1000 r--p 00000000 00:00 0                          [vvar]
    7ffdc77e1000-7ffdc77e3000 r-xp 00000000 00:00 0                          [vdso]
    ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]


    func mymsg ptr:0x4005f0
    in func:mymsg, msg:hello world, cnt:23031


