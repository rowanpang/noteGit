0,basic
    a,编译优化
    b,ref
        https://kernelnewbies.org/FAQ/LikelyUnlikely

1,together
    a,从if(b)开始,将所有成立时的分支放在一起
        一块执行,避免多次判断及跳转.

    b,./together 1 1

2,likely
    a,默认为likely 分支条件成立,分支汇编与上下文链在一起(增加cache命中率).
    b,只有当条件不成立时才jmp,发生跳转,刷新pipeline.

    c,x86_64
            if (likely(a)){
      40107f:       85 ed                   test   %ebp,%ebp
      401081:       74 21                   je     4010a4 <main+0x54>               #a 很多时候不为0,所以je(当为0)的时候跳转,
            printf("likely a\n");
      401083:       bf 10 20 40 00          mov    $0x402010,%edi
      401088:       e8 a3 ff ff ff          callq  401030 <puts@plt>
        } else {
            printf("likely !a\n");
        }

        if (unlikely(b)){
      40108d:       85 db                   test   %ebx,%ebx
      40108f:       75 1f                   jne    4010b0 <main+0x60>               #b 很多时候为0,所以jne(当不为0)的时候跳转,不刷新cpu pipeline.
            printf("unlikely b\n");
        } else {
            printf("unlikely !b\n");
      401091:       bf 2e 20 40 00          mov    $0x40202e,%edi                   #mov src,dst
      401096:       e8 95 ff ff ff          callq  401030 <puts@plt>
        }

    d,ppc64le
        if (likely(a)){
        100004fc:	00 00 9e 2f 	cmpwi   cr7,r30,0
        10000500:	b4 07 7f 7c 	extsw   r31,r3
        10000504:	48 00 de 41 	beq-    cr7,1000054c <main+0x9c>
            printf("likely a\n");
        10000508:	fe ff 62 3c 	addis   r3,r2,-2
        1000050c:	f8 89 63 38 	addi    r3,r3,-30216
        10000510:	81 ff ff 4b 	bl      10000490 <00000037.plt_call.puts@@GLIBC_2.17>
        10000514:	18 00 41 e8 	ld      r2,24(r1)                           #ld dst,src     24(r1) -> r2;
        } else {
            printf("likely !a\n");
        }

        if (unlikely(b)){
        10000518:	00 00 bf 2f 	cmpdi   cr7,r31,0
        1000051c:	44 00 de 40 	bne-    cr7,10000560 <main+0xb0>
            printf("unlikely b\n");
        } else {
            printf("unlikely !b\n");
        10000520:	fe ff 62 3c 	addis   r3,r2,-2
        10000524:	28 8a 63 38 	addi    r3,r3,-30168
        10000528:	69 ff ff 4b 	bl      10000490 <00000037.plt_call.puts@@GLIBC_2.17>
        1000052c:	18 00 41 e8 	ld      r2,24(r1)
        }

