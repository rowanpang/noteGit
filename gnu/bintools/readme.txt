两种stub函数,与编译选项之间的关系.
1,gcc -pg 
    000000000040069c <main>:
      40069c:	55                   	push   %rbp
      40069d:	48 89 e5             	mov    %rsp,%rbp
      4006a0:	48 83 ec 10          	sub    $0x10,%rsp
      4006a4:	e8 77 fe ff ff       	callq  400520 <mcount@plt>
      4006a9:	89 7d fc             	mov    %edi,-0x4(%rbp)
      4006ac:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
      4006b0:	e8 d1 ff ff ff       	callq  400686 <helloworld>
      4006b5:	b8 00 00 00 00       	mov    $0x0,%eax
      4006ba:	c9                   	leaveq 
      4006bb:	c3                   	retq   
      4006bc:	0f 1f 40 00          	nopl   0x0(%rax)

2,gcc -pg -mfentry
    00000000004006bc <main>:
      4006bc:	e8 8f fe ff ff       	callq  400550 <__fentry__@plt>
      4006c1:	55                   	push   %rbp
      4006c2:	48 89 e5             	mov    %rsp,%rbp
      4006c5:	48 83 ec 10          	sub    $0x10,%rsp
      4006c9:	89 7d fc             	mov    %edi,-0x4(%rbp)
      4006cc:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
      4006d0:	e8 d1 ff ff ff       	callq  4006a6 <helloworld>
      4006d5:	b8 00 00 00 00       	mov    $0x0,%eax
      4006da:	c9                   	leaveq 
      4006db:	c3                   	retq   
      4006dc:	0f 1f 40 00          	nopl   0x0(%rax)
