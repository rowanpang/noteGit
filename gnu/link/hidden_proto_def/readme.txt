
cmd:
	gcc -shared -fPIC ./hello.c -o libhello.so
	gcc main.c -L./ -lhello -Wl,-rpath=/home/pangwz/test/gcc/sec_link_

for libhello.so
	if has,not need plt
		00000000000005e1 <print_1>:
		55                   	push   %rbp
		48 89 e5             	mov    %rsp,%rbp
		b8 00 00 00 00       	mov    $0x0,%eax
		e8 dd ff ff ff       	callq  5cc <print>
		c9                   	leaveq 
		c3                   	retq   
	if no,need plt
		0000000000000611 <print_1>:
		55                   	push   %rbp
		48 89 e5             	mov    %rsp,%rbp
		b8 00 00 00 00       	mov    $0x0,%eax
		e8 f9 fe ff ff       	callq  518 <print@plt>
		c9                   	leaveq 

	
	
