1,elf exec 视图说明

2,进程的 stack 栈段由操作系统自动分配.
	在代码中，编译器使用sp 的增加/减小来获取可以使用的内存空间，称之为栈空间.
    exp: main()中的 int ret，局部变量,其对应的 asm 为:
		00000000004004e6 <main>:

		int main(int argc,char **argv)
		{
		  4004e6:	55                   	push   %rbp
		  4004e7:	48 89 e5             	mov    %rsp,%rbp
		  4004ea:	48 83 ec 20          	sub    $0x20,%rsp        //预留了0x20字节的stack空间.
		  4004ee:	89 7d ec             	mov    %edi,-0x14(%rbp)
		  4004f1:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
			int ret = 0;
		  4004f5:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)   //ret 变量使用了 -0x4(%rbp),这是编译器自动分配的，后面所有涉及ret的操作都要使用 -0x4(%ebp)
			printf("in main \n");
		  4004fc:	bf 20 06 40 00       	mov    $0x400620,%edi
		  400501:	e8 b2 fe ff ff       	callq  4003b8 <puts@plt>
			
			show(1);
		  400506:	bf 01 00 00 00       	mov    $0x1,%edi
		  40050b:	e8 b4 ff ff ff       	callq  4004c4 <show>

			return ret;
		  400510:	8b 45 fc             	mov    -0x4(%rbp),%eax //调用 ret  -0x4(%ebp)
		}
	
3,主要的segment
	Program Headers:
	  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
	  PHDR           0x000041 0x0000000000400040 0x0000000000400040 0x0001c0 0x0001c0 R E 0x8				program header segment
	  INTERP         0x000200 0x0000000000400200 0x0000000000400200 0x00001c 0x00001c R   0x1				interpreter ,解析器信息 段
		  [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
	  LOAD           0x000000 0x0000000000400000 0x0000000000400000 0x0006f4 0x0006f4 R E 0x200000			包含，代码/数据/只读等
	  LOAD           0x0006f8 0x00000000006006f8 0x00000000006006f8 0x0001ec 0x000200 RW  0x200000
	  DYNAMIC        0x000720 0x0000000000600720 0x0000000000600720 0x000190 0x000190 RW  0x8
	  NOTE           0x00021c 0x000000000040021c 0x000000000040021c 0x000044 0x000044 R   0x4
	  GNU_EH_FRAME   0x00062c 0x000000000040062c 0x000000000040062c 0x00002c 0x00002c R   0x4
	  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x8

	 Section to Segment mapping:
	  Segment Sections...
	   00     
	   01     .interp 
	   02     .interp .note.ABI-tag .note.gnu.build-id .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt .init .plt .text .fini .rodata .eh_frame_hdr .eh_frame 
	   03     .ctors .dtors .jcr .dynamic .got .got.plt .data .bss 
	   04     .dynamic 
	   05     .note.ABI-tag .note.gnu.build-id 
	   06     .eh_frame_hdr 
	   07     

