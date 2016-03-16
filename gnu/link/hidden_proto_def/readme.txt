for libhello.so
	if has,not need plt
		00000000000005e1 <print_1>:
		55                   	push   %rbp
		48 89 e5             	mov    %rsp,%rbp
		b8 00 00 00 00       	mov    $0x0,%eax
		e8 dd ff ff ff       	callq  5cc <print>
		c9                   	leaveq 
		c3                   	retq   

		44: 000000000000061c    21 FUNC    LOCAL  DEFAULT   11 print_internal
		57: 000000000000061c    21 FUNC    GLOBAL DEFAULT   11 print

		hello.s
			.globl print_internal
				.hidden print_internal
				.type   print_internal, @function
			print_internal: <--------------- 
			.LFB0:                          \
				cfi_startproc               |
				pushq   %rbp                |
				.......                     |
			print_1:                        |
			.LFB2:                          |
				.......                     |
				call    print_internal ----/在asm中所有的print symbol都换成了 print_internal
				.......						所以在链接时so内部都是用print_internal链接,而print_internal是hidden(local)的所以不需要plt

			#APP
			.global print
			.set print, print_internal //定义输出global symbol print, 方便so外部的调用

	if no,need plt
		0000000000000611 <print_1>:
		55                   	push   %rbp
		48 89 e5             	mov    %rsp,%rbp
		b8 00 00 00 00       	mov    $0x0,%eax
		e8 f9 fe ff ff       	callq  518 <print@plt>
		c9                   	leaveq 

	
	
