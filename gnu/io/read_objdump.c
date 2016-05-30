
a.out:     file format elf64-x86-64
a.out


Disassembly of section .init:

0000000000400438 <_init>:
  400438:	48 83 ec 08          	sub    $0x8,%rsp
  40043c:	e8 9b 00 00 00       	callq  4004dc <call_gmon_start>
  400441:	e8 2a 01 00 00       	callq  400570 <frame_dummy>
  400446:	e8 65 02 00 00       	callq  4006b0 <__do_global_ctors_aux>
  40044b:	48 83 c4 08          	add    $0x8,%rsp
  40044f:	c3                   	retq   

Disassembly of section .plt:

0000000000400450 <printf@plt-0x10>:
  400450:	ff 35 b2 05 20 00    	pushq  0x2005b2(%rip)        # 600a08 <_GLOBAL_OFFSET_TABLE_+0x8>
  400456:	ff 25 b4 05 20 00    	jmpq   *0x2005b4(%rip)        # 600a10 <_GLOBAL_OFFSET_TABLE_+0x10>
  40045c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400460 <printf@plt>:
  400460:	ff 25 b2 05 20 00    	jmpq   *0x2005b2(%rip)        # 600a18 <_GLOBAL_OFFSET_TABLE_+0x18>
  400466:	68 00 00 00 00       	pushq  $0x0
  40046b:	e9 e0 ff ff ff       	jmpq   400450 <_init+0x18>

0000000000400470 <puts@plt>:
  400470:	ff 25 aa 05 20 00    	jmpq   *0x2005aa(%rip)        # 600a20 <_GLOBAL_OFFSET_TABLE_+0x20>
  400476:	68 01 00 00 00       	pushq  $0x1
  40047b:	e9 d0 ff ff ff       	jmpq   400450 <_init+0x18>

0000000000400480 <read@plt>:
  400480:	ff 25 a2 05 20 00    	jmpq   *0x2005a2(%rip)        # 600a28 <_GLOBAL_OFFSET_TABLE_+0x28>
  400486:	68 02 00 00 00       	pushq  $0x2
  40048b:	e9 c0 ff ff ff       	jmpq   400450 <_init+0x18>

0000000000400490 <__libc_start_main@plt>:
  400490:	ff 25 9a 05 20 00    	jmpq   *0x20059a(%rip)        # 600a30 <_GLOBAL_OFFSET_TABLE_+0x30>
  400496:	68 03 00 00 00       	pushq  $0x3
  40049b:	e9 b0 ff ff ff       	jmpq   400450 <_init+0x18>

00000000004004a0 <write@plt>:
  4004a0:	ff 25 92 05 20 00    	jmpq   *0x200592(%rip)        # 600a38 <_GLOBAL_OFFSET_TABLE_+0x38>
  4004a6:	68 04 00 00 00       	pushq  $0x4
  4004ab:	e9 a0 ff ff ff       	jmpq   400450 <_init+0x18>

Disassembly of section .text:

00000000004004b0 <_start>:
  4004b0:	31 ed                	xor    %ebp,%ebp
  4004b2:	49 89 d1             	mov    %rdx,%r9
  4004b5:	5e                   	pop    %rsi
  4004b6:	48 89 e2             	mov    %rsp,%rdx
  4004b9:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  4004bd:	50                   	push   %rax
  4004be:	54                   	push   %rsp
  4004bf:	49 c7 c0 10 06 40 00 	mov    $0x400610,%r8
  4004c6:	48 c7 c1 20 06 40 00 	mov    $0x400620,%rcx
  4004cd:	48 c7 c7 dd 05 40 00 	mov    $0x4005dd,%rdi
  4004d4:	e8 b7 ff ff ff       	callq  400490 <__libc_start_main@plt>
  4004d9:	f4                   	hlt    
  4004da:	90                   	nop
  4004db:	90                   	nop

00000000004004dc <call_gmon_start>:
  4004dc:	48 83 ec 08          	sub    $0x8,%rsp
  4004e0:	48 8b 05 11 05 20 00 	mov    0x200511(%rip),%rax        # 6009f8 <_DYNAMIC+0x190>
  4004e7:	48 85 c0             	test   %rax,%rax
  4004ea:	74 02                	je     4004ee <call_gmon_start+0x12>
  4004ec:	ff d0                	callq  *%rax
  4004ee:	48 83 c4 08          	add    $0x8,%rsp
  4004f2:	c3                   	retq   
  4004f3:	90                   	nop
  4004f4:	90                   	nop
  4004f5:	90                   	nop
  4004f6:	90                   	nop
  4004f7:	90                   	nop
  4004f8:	90                   	nop
  4004f9:	90                   	nop
  4004fa:	90                   	nop
  4004fb:	90                   	nop
  4004fc:	90                   	nop
  4004fd:	90                   	nop
  4004fe:	90                   	nop
  4004ff:	90                   	nop

0000000000400500 <__do_global_dtors_aux>:
  400500:	55                   	push   %rbp
  400501:	48 89 e5             	mov    %rsp,%rbp
  400504:	53                   	push   %rbx
  400505:	48 83 ec 08          	sub    $0x8,%rsp
  400509:	80 3d 38 05 20 00 00 	cmpb   $0x0,0x200538(%rip)        # 600a48 <completed.6349>
  400510:	75 4b                	jne    40055d <__do_global_dtors_aux+0x5d>
  400512:	bb 58 08 60 00       	mov    $0x600858,%ebx
  400517:	48 8b 05 32 05 20 00 	mov    0x200532(%rip),%rax        # 600a50 <dtor_idx.6351>
  40051e:	48 81 eb 50 08 60 00 	sub    $0x600850,%rbx
  400525:	48 c1 fb 03          	sar    $0x3,%rbx
  400529:	48 83 eb 01          	sub    $0x1,%rbx
  40052d:	48 39 d8             	cmp    %rbx,%rax
  400530:	73 24                	jae    400556 <__do_global_dtors_aux+0x56>
  400532:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
  400538:	48 83 c0 01          	add    $0x1,%rax
  40053c:	48 89 05 0d 05 20 00 	mov    %rax,0x20050d(%rip)        # 600a50 <dtor_idx.6351>
  400543:	ff 14 c5 50 08 60 00 	callq  *0x600850(,%rax,8)
  40054a:	48 8b 05 ff 04 20 00 	mov    0x2004ff(%rip),%rax        # 600a50 <dtor_idx.6351>
  400551:	48 39 d8             	cmp    %rbx,%rax
  400554:	72 e2                	jb     400538 <__do_global_dtors_aux+0x38>
  400556:	c6 05 eb 04 20 00 01 	movb   $0x1,0x2004eb(%rip)        # 600a48 <completed.6349>
  40055d:	48 83 c4 08          	add    $0x8,%rsp
  400561:	5b                   	pop    %rbx
  400562:	c9                   	leaveq 
  400563:	c3                   	retq   
  400564:	66 66 66 2e 0f 1f 84 	data32 data32 nopw %cs:0x0(%rax,%rax,1)
  40056b:	00 00 00 00 00 

0000000000400570 <frame_dummy>:
  400570:	48 83 3d e8 02 20 00 	cmpq   $0x0,0x2002e8(%rip)        # 600860 <__JCR_END__>
  400577:	00 
  400578:	55                   	push   %rbp
  400579:	48 89 e5             	mov    %rsp,%rbp
  40057c:	74 12                	je     400590 <frame_dummy+0x20>
  40057e:	b8 00 00 00 00       	mov    $0x0,%eax
  400583:	48 85 c0             	test   %rax,%rax
  400586:	74 08                	je     400590 <frame_dummy+0x20>
  400588:	bf 60 08 60 00       	mov    $0x600860,%edi
  40058d:	c9                   	leaveq 
  40058e:	ff e0                	jmpq   *%rax
  400590:	c9                   	leaveq 
  400591:	c3                   	retq   
  400592:	90                   	nop
  400593:	90                   	nop

0000000000400594 <impossible>:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void impossible()
{
  400594:	55                   	push   %rbp
  400595:	48 89 e5             	mov    %rsp,%rbp
	printf("Turn the impossible into possible!\n");
  400598:	bf 08 07 40 00       	mov    $0x400708,%edi
  40059d:	e8 ce fe ff ff       	callq  400470 <puts@plt>
}
  4005a2:	c9                   	leaveq 
  4005a3:	c3                   	retq   

00000000004005a4 <vulnerable_function>:

void vulnerable_function() {
  4005a4:	55                   	push   %rbp
  4005a5:	48 89 e5             	mov    %rsp,%rbp
  4005a8:	48 83 c4 80          	add    $0xffffffffffffff80,%rsp
	char buf[128];
	read(STDIN_FILENO, buf, 64);
  4005ac:	48 8d 45 80          	lea    -0x80(%rbp),%rax
  4005b0:	ba 40 00 00 00       	mov    $0x40,%edx
  4005b5:	48 89 c6             	mov    %rax,%rsi
  4005b8:	bf 00 00 00 00       	mov    $0x0,%edi
  4005bd:	e8 be fe ff ff       	callq  400480 <read@plt>
	printf("after read %s",buf);
  4005c2:	b8 2b 07 40 00       	mov    $0x40072b,%eax
  4005c7:	48 8d 55 80          	lea    -0x80(%rbp),%rdx
  4005cb:	48 89 d6             	mov    %rdx,%rsi
  4005ce:	48 89 c7             	mov    %rax,%rdi
  4005d1:	b8 00 00 00 00       	mov    $0x0,%eax
  4005d6:	e8 85 fe ff ff       	callq  400460 <printf@plt>
}
  4005db:	c9                   	leaveq 
  4005dc:	c3                   	retq   

00000000004005dd <main>:

int main(int argc, char** argv) {
  4005dd:	55                   	push   %rbp
  4005de:	48 89 e5             	mov    %rsp,%rbp
  4005e1:	48 83 ec 10          	sub    $0x10,%rsp
  4005e5:	89 7d fc             	mov    %edi,-0x4(%rbp)
  4005e8:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
	write(STDOUT_FILENO, "Hello, World\n", 13);
  4005ec:	ba 0d 00 00 00       	mov    $0xd,%edx
  4005f1:	be 39 07 40 00       	mov    $0x400739,%esi
  4005f6:	bf 01 00 00 00       	mov    $0x1,%edi
  4005fb:	e8 a0 fe ff ff       	callq  4004a0 <write@plt>
	vulnerable_function();
  400600:	b8 00 00 00 00       	mov    $0x0,%eax
  400605:	e8 9a ff ff ff       	callq  4005a4 <vulnerable_function>
}
  40060a:	c9                   	leaveq 
  40060b:	c3                   	retq   
  40060c:	90                   	nop
  40060d:	90                   	nop
  40060e:	90                   	nop
  40060f:	90                   	nop

0000000000400610 <__libc_csu_fini>:
  400610:	f3 c3                	repz retq 
  400612:	66 66 66 66 66 2e 0f 	data32 data32 data32 data32 nopw %cs:0x0(%rax,%rax,1)
  400619:	1f 84 00 00 00 00 00 

0000000000400620 <__libc_csu_init>:
  400620:	48 89 6c 24 d8       	mov    %rbp,-0x28(%rsp)
  400625:	4c 89 64 24 e0       	mov    %r12,-0x20(%rsp)
  40062a:	48 8d 2d 0b 02 20 00 	lea    0x20020b(%rip),%rbp        # 60083c <__init_array_end>
  400631:	4c 8d 25 04 02 20 00 	lea    0x200204(%rip),%r12        # 60083c <__init_array_end>
  400638:	4c 89 6c 24 e8       	mov    %r13,-0x18(%rsp)
  40063d:	4c 89 74 24 f0       	mov    %r14,-0x10(%rsp)
  400642:	4c 89 7c 24 f8       	mov    %r15,-0x8(%rsp)
  400647:	48 89 5c 24 d0       	mov    %rbx,-0x30(%rsp)
  40064c:	48 83 ec 38          	sub    $0x38,%rsp
  400650:	4c 29 e5             	sub    %r12,%rbp
  400653:	41 89 fd             	mov    %edi,%r13d
  400656:	49 89 f6             	mov    %rsi,%r14
  400659:	48 c1 fd 03          	sar    $0x3,%rbp
  40065d:	49 89 d7             	mov    %rdx,%r15
  400660:	e8 d3 fd ff ff       	callq  400438 <_init>
  400665:	48 85 ed             	test   %rbp,%rbp
  400668:	74 1c                	je     400686 <__libc_csu_init+0x66>
  40066a:	31 db                	xor    %ebx,%ebx
  40066c:	0f 1f 40 00          	nopl   0x0(%rax)
  400670:	4c 89 fa             	mov    %r15,%rdx
  400673:	4c 89 f6             	mov    %r14,%rsi
  400676:	44 89 ef             	mov    %r13d,%edi
  400679:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40067d:	48 83 c3 01          	add    $0x1,%rbx
  400681:	48 39 eb             	cmp    %rbp,%rbx
  400684:	72 ea                	jb     400670 <__libc_csu_init+0x50>
  400686:	48 8b 5c 24 08       	mov    0x8(%rsp),%rbx
  40068b:	48 8b 6c 24 10       	mov    0x10(%rsp),%rbp
  400690:	4c 8b 64 24 18       	mov    0x18(%rsp),%r12
  400695:	4c 8b 6c 24 20       	mov    0x20(%rsp),%r13
  40069a:	4c 8b 74 24 28       	mov    0x28(%rsp),%r14
  40069f:	4c 8b 7c 24 30       	mov    0x30(%rsp),%r15
  4006a4:	48 83 c4 38          	add    $0x38,%rsp
  4006a8:	c3                   	retq   
  4006a9:	90                   	nop
  4006aa:	90                   	nop
  4006ab:	90                   	nop
  4006ac:	90                   	nop
  4006ad:	90                   	nop
  4006ae:	90                   	nop
  4006af:	90                   	nop

00000000004006b0 <__do_global_ctors_aux>:
  4006b0:	55                   	push   %rbp
  4006b1:	48 89 e5             	mov    %rsp,%rbp
  4006b4:	53                   	push   %rbx
  4006b5:	48 83 ec 08          	sub    $0x8,%rsp
  4006b9:	48 8b 05 80 01 20 00 	mov    0x200180(%rip),%rax        # 600840 <__CTOR_LIST__>
  4006c0:	48 83 f8 ff          	cmp    $0xffffffffffffffff,%rax
  4006c4:	74 19                	je     4006df <__do_global_ctors_aux+0x2f>
  4006c6:	bb 40 08 60 00       	mov    $0x600840,%ebx
  4006cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4006d0:	48 83 eb 08          	sub    $0x8,%rbx
  4006d4:	ff d0                	callq  *%rax
  4006d6:	48 8b 03             	mov    (%rbx),%rax
  4006d9:	48 83 f8 ff          	cmp    $0xffffffffffffffff,%rax
  4006dd:	75 f1                	jne    4006d0 <__do_global_ctors_aux+0x20>
  4006df:	48 83 c4 08          	add    $0x8,%rsp
  4006e3:	5b                   	pop    %rbx
  4006e4:	c9                   	leaveq 
  4006e5:	c3                   	retq   
  4006e6:	90                   	nop
  4006e7:	90                   	nop

Disassembly of section .fini:

00000000004006e8 <_fini>:
  4006e8:	48 83 ec 08          	sub    $0x8,%rsp
  4006ec:	e8 0f fe ff ff       	callq  400500 <__do_global_dtors_aux>
  4006f1:	48 83 c4 08          	add    $0x8,%rsp
  4006f5:	c3                   	retq   
