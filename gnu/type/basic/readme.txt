1,_Bool
    a,gnu extention 
    b,汇编时已经决定了1 or 0.

2,implement
    85	    int a = 2;
    0x000000000040084c <+742>:	movl   $0x2,-0x4(%rbp)		#编译时已经确定了value,直接赋1

    86	    _Bool testBool=3;
    0x0000000000400853 <+749>:	movb   $0x1,-0x5(%rbp)

    87	    _Bool testBool2=a;
    0x0000000000400857 <+753>:	cmpl   $0x0,-0x4(%rbp)		#判断a是否等于0.
    0x000000000040085b <+757>:	setne  %al			#如果不等于0,将al=1,置位.
    0x000000000040085e <+760>:	mov    %al,-0x6(%rbp)		#将al的值放到testBool2中,实现_Bool 类型变量.

    88	    printf("sizeof(testBool):%d,value:%d\n",sizeof(testBool),testBool);

