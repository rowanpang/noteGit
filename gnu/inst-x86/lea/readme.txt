0.总结:
    b,看如下汇编源码与编译后的对比:

    a,lea 取操作数的有效地址,有效地址的表达方式有多种
	lea _start(%rip),%rax :
	    使用%rip相对寻址,得到_start的有效地址.
	    目的是将_start的有效地址放入rax，确切将是运行时的有效地址.
	    汇编之后等消于:_start相对于next inst的地址 + %rip -> %eax
	lea _sys(%ebx),%rax :
	    使用%ebx相对寻址，ebx不会像rip随指令执行而变化，可以是一个固定值
	    此处就不能按照上面的理解

1,asm source code:
	.global _start
	.text
	_start:
		mov $3,%ebx
		leaq _start(%rip), %rax <--------------------------------------------------------\
		leaq _sys(%rip),%rax <-----------------------------------------------------------+-\
		leaq _sys(%ebx),%rax <-----------------------------------------------------------+-+-\
		mov $1,%eax			#syscall write					 | | |
		mov $1,%edi			#stdout                                          | | |
		mov $message, %rsi	#source string                                           | | |
		mov $len,%edx		#len                                                     | | |
	_sys:											 | | |
		syscall                                                                          | | |
		.....                                                                            | | |
												 | | |
2,gdb disassemble										 | | |
	Dump of assembler code for function _start:						 | | |
	=> 0x0000000000400078 <+0>:		mov    $0x3,%ebx                                 | | |
	   0x000000000040007d <+5>:		lea    -0xc(%rip),%rax      0x400078 <_start>  -/  | |
	   0x0000000000400084 <+12>:	lea    0x1e(%rip),%rax            # 0x4000a9 <_sys> ------/  |
	   0x000000000040008b <+19>:	lea    0x4000a9(%ebx),%rax     -----------------------------/
	   0x0000000000400093 <+27>:	mov    $0x1,%eax
	   0x0000000000400098 <+32>:	mov    $0x1,%edi
	   0x000000000040009d <+37>:	mov    $0x4000b5,%rsi
	   0x00000000004000a4 <+44>:	mov    $0x14,%edx

