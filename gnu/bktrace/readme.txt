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
    a, $ ./backtrace
	=========>>>catch signal 11 <<<=========
	Dump stack start...
	backtrace() returned 8 addresses
	  [00] ./backtrace(dump+0x1f) [0x400a03]
	  [01] ./backtrace(signal_handler+0x2e) [0x400aca]
	  [02] /lib64/libc.so.6(+0x36a80) [0x7f0ea3428a80]
	  [03] ./libadd.so(add1+0x1a) [0x7f0ea37c7624]
	  [04] ./libadd.so(add+0x1c) [0x7f0ea37c7659]
	  [05] ./backtrace(main+0x2f) [0x4009c6]
	  [06] /lib64/libc.so.6(__libc_start_main+0xea) [0x7f0ea341288a]
	  [07] ./backtrace(_start+0x2a) [0x4008ea]
	Dump stack end...
	[1]    9263 segmentation fault (core dumped)  ./backtrace

    b,利用maps解码
	 Maps信息第一项表示的为地址范围如第一条记录中的7f0962fb3000-7f0962fb4000，第二项r-xp分别表示只读、可执行、私有的，由此可知这里存放的为libadd.so的.text段即代码段，后面的栈信息0x7f0962fb35c6也正好是落在了这个区间。所有我们正确的地址应为0x7f0962fb35c6 - 7f0962fb3000 = 0x5c6，将这个地址利用addr2line命令得到如下结果：

	zoulm@zoulm-VirtualBox:/home/share/work/backtrace$ addr2line -e libadd.so 0x5c6
	/home/share/work/backtrace/add.c:13

