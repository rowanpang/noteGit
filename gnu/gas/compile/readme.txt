1,diff hello.s gcc-hello.s
	查看使用gcc 与 glibc库连接编译 与 
	单独分步编译的差别.


2,static VS dynamic
	[pangwz@rowanInspur sperate]$ ldd hello.out 
		not a dynamic executable
	[pangwz@rowanInspur sperate]$ ldd gcc-hello.out 
		linux-vdso.so.1 (0x00007ffec4352000)
		libc.so.6 => /lib64/libc.so.6 (0x00007f292c5d2000)
		/lib64/ld-linux-x86-64.so.2 (0x0000558a08493000)
	[pangwz@rowanInspur sperate]$ 

	
