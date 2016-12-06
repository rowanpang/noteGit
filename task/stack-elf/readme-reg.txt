reg:
	相对于layout,是后面重新执行补充的,地址对不上,源代码也改动过,仅仅做参考.
	Breakpoint 1, main (argc=2, argv=0x7fffffffe0c8) at main.c:21
	(gdb) info reg
	rax            0x400618	4195864
	rbx            0x0	0
	rcx            0x4006d0	4196048
	rdx            0x7fffffffe0e0	140737488347360
	rsi            0x7fffffffe0c8	140737488347336
	rdi            0x2	2
	rbp            0x7fffffffdfe0	0x7fffffffdfe0
	rsp            0x7fffffffdfc0	0x7fffffffdfc0
	r8             0x7ffff7dd35d8	140737351857624
	r9             0x7ffff7de91d0	140737351946704
	r10            0x846	2118
	r11            0x7ffff7a36640	140737348068928
	r12            0x400500	4195584
	r13            0x7fffffffe0c0	140737488347328
	r14            0x0	0
	r15            0x0	0
	rip            0x400627	0x400627 <main+15>
	eflags         0x206	[ PF IF ]
	cs             0x33	51
	ss             0x2b	43
	ds             0x0	0
	es             0x0	0
	fs             0x0	0
	gs             0x0	0

	重点:rpb rsp
		rbp            0x7fffffffdfe0	0x7fffffffdfe0
		rsp            0x7fffffffdfc0	0x7fffffffdfc0
		0x7fffffffe0c8 - %rpb = E8, 即rbp还在argv的下方E8处.
