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
  401091:       bf 2e 20 40 00          mov    $0x40202e,%edi
  401096:       e8 95 ff ff ff          callq  401030 <puts@plt>
    }
