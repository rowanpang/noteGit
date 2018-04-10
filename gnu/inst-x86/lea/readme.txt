0.总结:
    a,将操作数的结果作为地址,而不是取地址里的值.

    b,lea 取操作数的有效地址,有效地址的表达方式有多种
	lea _start(%rip),%rax :
	    使用%rip相对寻址,得到_start的有效地址.
	    目的是将_start的有效地址放入rax，确切说是运行时的有效地址.
	    汇编之后等消于:_start相对于next inst的地址 + %rip -> %eax
	lea _sys(%ebx),%rax :
	    使用%ebx相对寻址,ebx不会像rip随指令执行而变化,可以是一个固定值

	lea 0xe0(%rsp),%rax:
	    将rsp+0xe0的值放到rax中,而不是[rsp+0x30]处的值.

1,
    (gdb) disassemble _start
	Dump of assembler code for function _start:
	   0x0000000000400078 <+0>:	    mov    $0x3,%ebx
	   0x000000000040007d <+5>:	    lea    -0xc(%rip),%rax
			#rax中的值为,0x400078 <_start>
	   0x0000000000400084 <+12>:    lea    0x20(%rip),%rax
			#rax中的值为,0x4000ab <_sys>
	   0x000000000040008b <+19>:    lea    0x4000ab(%ebx),%rax
			#rax中的值为,0x4000ae = 0x4000ab+%ebx
	   0x0000000000400093 <+27>:    mov    $0x1,%eax
	   0x0000000000400098 <+32>:    mov    $0x1,%edi
	   0x000000000040009d <+37>:    mov    $0x4000b5,%rsi
	   0x00000000004000a4 <+44>:    mov    $0x14,%edx
	   0x00000000004000a9 <+49>:    syscall
	End of assembler dump.
    (gdb) c
	Continuing.
	Breakpoint 2, 0x0000000000400084 in _start ()
    (gdb) info registers rax
	rax            0x400078	4194424
    (gdb) c
	Continuing.
	Breakpoint 3, 0x000000000040008b in _start ()
    (gdb) info registers rax
	rax            0x4000ab	4194475
    (gdb) c
	Continuing.
	Breakpoint 4, 0x0000000000400093 in _start ()
    (gdb) info registers rax rbx
	rax            0x4000ae	    4194478
	rbx            0x3	    3
