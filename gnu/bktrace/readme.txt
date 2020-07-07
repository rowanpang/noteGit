0,basic
    a,https://blog.csdn.net/jxgz_leo/article/details/53458366

1,one file static
    a,
	$ ./a.out
	    =========>>>catch signal 11 <<<=========
	    Dump stack start...
	    backtrace() returned 8 addresses
	      [00] ./a.out(dump+0x1f) [0x4009f3]
	      [01] ./a.out(signal_handler+0x2e) [0x400aba]
	      [02] /lib64/libc.so.6(+0x36a80) [0x7f77a60e4a80]
	      [03] ./a.out(add1+0x1a) [0x400afa]
	      [04] ./a.out(add+0x1c) [0x400b2f]
	      [05] ./a.out(main+0x2f) [0x4009b6]
	      [06] /lib64/libc.so.6(__libc_start_main+0xea) [0x7f77a60ce88a]
	      [07] ./a.out(_start+0x2a) [0x4008da]
	    Dump stack end...
	    [1]    7148 segmentation fault (core dumped)  ./a.out

	$ addr2line -e a.out 0x400afa
	    /root/noteGit/gnu/bktrace/add.c:10


2,dynamic
    a,
	$ ./backtrace

	=========>>>catch signal 11 <<<=========
	Dump stack start...
	00400000-00401000 r-xp 00000000 fd:00 6691061                            /root/noteGit/gnu/bktrace/backtrace
	00601000-00602000 r--p 00001000 fd:00 6691061                            /root/noteGit/gnu/bktrace/backtrace
	00602000-00603000 rw-p 00002000 fd:00 6691061                            /root/noteGit/gnu/bktrace/backtrace
	01628000-01649000 rw-p 00000000 00:00 0                                  [heap]
	7f26ce185000-7f26ce350000 r-xp 00000000 fd:00 10756900                   /usr/lib64/libc-2.25.so
	7f26ce350000-7f26ce550000 ---p 001cb000 fd:00 10756900                   /usr/lib64/libc-2.25.so
	7f26ce550000-7f26ce554000 r--p 001cb000 fd:00 10756900                   /usr/lib64/libc-2.25.so
	7f26ce554000-7f26ce556000 rw-p 001cf000 fd:00 10756900                   /usr/lib64/libc-2.25.so
	7f26ce556000-7f26ce55a000 rw-p 00000000 00:00 0
	7f26ce55a000-7f26ce55b000 r-xp 00000000 fd:00 6691058                    /root/noteGit/gnu/bktrace/libadd.so
	7f26ce55b000-7f26ce75a000 ---p 00001000 fd:00 6691058                    /root/noteGit/gnu/bktrace/libadd.so
	7f26ce75a000-7f26ce75b000 r--p 00000000 fd:00 6691058                    /root/noteGit/gnu/bktrace/libadd.so
	7f26ce75b000-7f26ce75c000 rw-p 00001000 fd:00 6691058                    /root/noteGit/gnu/bktrace/libadd.so
	7f26ce75c000-7f26ce782000 r-xp 00000000 fd:00 10758760                   /usr/lib64/ld-2.25.so
	7f26ce94e000-7f26ce951000 rw-p 00000000 00:00 0
	7f26ce97f000-7f26ce981000 rw-p 00000000 00:00 0
	7f26ce981000-7f26ce982000 r--p 00025000 fd:00 10758760                   /usr/lib64/ld-2.25.so
	7f26ce982000-7f26ce984000 rw-p 00026000 fd:00 10758760                   /usr/lib64/ld-2.25.so
	7ffddbb77000-7ffddbb98000 rw-p 00000000 00:00 0                          [stack]
	7ffddbbb3000-7ffddbbb5000 r--p 00000000 00:00 0                          [vvar]
	7ffddbbb5000-7ffddbbb7000 r-xp 00000000 00:00 0                          [vdso]
	ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
	backtrace() returned 8 addresses
	  [00] ./backtrace(dump+0xa6) [0x400b6a]
	  [01] ./backtrace(signal_handler+0x2e) [0x400c31]
	  [02] /lib64/libc.so.6(+0x36a80) [0x7f26ce1bba80]
	  [03] ./libadd.so(add1+0x1a) [0x7f26ce55a624]				# add1 func 之后执行了signal_handler,即出现了coredump.
	  [04] ./libadd.so(add+0x1c) [0x7f26ce55a659]
	  [05] ./backtrace(main+0x2f) [0x400aa6]
	  [06] /lib64/libc.so.6(__libc_start_main+0xea) [0x7f26ce1a588a]
	  [07] ./backtrace(_start+0x2a) [0x4009ca]
	Dump stack end...
	[1]    29055 segmentation fault (core dumped)  ./backtrace

    b,利用maps解码
	Maps信息 第二项r-xp分别表示只读、可执行、私有的.
	由此可知这里存放的为libadd.so的.text段即代码段，
	    7f26ce55a000-7f26ce55b000 r-xp 00000000 fd:00 6691058                    /root/noteGit/gnu/bktrace/libadd.so
	symbol 在so中的偏移地址为:
	    0x7f26ce55a624 - 0x7f26ce55a000 = 0x624.将这个地址利用addr2line命令得到如下结果：
	$ addr2line -e libadd.so 0x624
	    /root/noteGit/gnu/bktrace/add.c:10

    c,gdb coredump
	$ coredumpctl gdb /root/noteGit/gnu/bktrace/backtrace
		       PID: 29055 (backtrace)
		       UID: 0 (root)
		       GID: 0 (root)
		    Signal: 11 (SEGV)
		 Timestamp: Tue 2020-07-07 10:23:53 CST (20min ago)
	      Command Line: ./backtrace
		Executable: /root/noteGit/gnu/bktrace/backtrace
	     Control Group: /user.slice/user-0.slice/session-1.scope
		      Unit: session-1.scope
		     Slice: user-0.slice
		   Session: 1
		 Owner UID: 0 (root)
		   Boot ID: 24ca29f4bfc74e94b6ab864e8bce62af
		Machine ID: 2577f25f4874499785ed9ffb86167959
		  Hostname: rowanInspur
		   Storage: /var/lib/systemd/coredump/core.backtrace.0.24ca29f4bfc74e94b6ab864e8bce62af.29055.1594088633000000.lz4
		   Message: Process 29055 (backtrace) of user 0 dumped core.

			    Stack trace of thread 29055:
			    #0  0x00007f26ce55a624 n/a (/root/noteGit/gnu/bktrace/libadd.so)
			    #1  0x00007f26ce55a659 n/a (/root/noteGit/gnu/bktrace/libadd.so)
			    #2  0x0000000000400aa6 n/a (/root/noteGit/gnu/bktrace/backtrace)
			    #3  0x00007f26ce1a588a __libc_start_main (libc.so.6)
			    #4  0x00000000004009ca n/a (/root/noteGit/gnu/bktrace/backtrace)

	    GNU gdb (GDB) Fedora 8.0-13.fc26
	    Copyright (C) 2017 Free Software Foundation, Inc.
	    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
	    This is free software: you are free to change and redistribute it.
	    There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
	    and "show warranty" for details.
	    This GDB was configured as "x86_64-redhat-linux-gnu".
	    Type "show configuration" for configuration details.
	    For bug reporting instructions, please see:
	    <http://www.gnu.org/software/gdb/bugs/>.
	    Find the GDB manual and other documentation resources online at:
	    <http://www.gnu.org/software/gdb/documentation/>.
	    For help, type "help".
	    Type "apropos word" to search for commands related to "word"...
	    Reading symbols from /root/noteGit/gnu/bktrace/backtrace...done.
	    [New LWP 29055]
	    Core was generated by `./backtrace'.
	    Program terminated with signal SIGSEGV, Segmentation fault.
	    #0  0x00007f26ce55a624 in add1 (num=0) at add.c:10
	    10		*pTemp = 0x01;  /* 这将导致一个段错误，致使程序崩溃退出 */
	    Missing separate debuginfos, use: dnf debuginfo-install libgcc-7.1.1-3.fc26.x86_64
	    (gdb) bt
	    #0  0x00007f26ce55a624 in add1 (num=0) at add.c:10
	    #1  0x00007f26ce55a659 in add (num=0) at add.c:21
	    #2  0x0000000000400aa6 in main (argc=1, argv=0x7ffddbb95578) at main.c:17
	    (gdb)
