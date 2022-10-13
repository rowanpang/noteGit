0,basic
    a,通过hidden 避免so内部的 plt (Procedure Link Table);
    b,libhello.so 实现2个func()
        1),print()
        2),print_1() -call-> print()        #目的是避免这里的plt

    c,实现方式
        1),通过asm()定义一个fct_internal,并hidden 这个fct_internal
        2),通过def global fct,再把fct_internal 以fct symbol的方式对外export

    d,如果把
        __hidden_proto(xx) 中的hidden去掉,则print_internal 被export.
        print_1:
            ...
            call    print_internal@PLT                          #需要经过plt

1,if hidden
    a,$ readelf --symbols libhello.so
        40: 0000000000001109    22 FUNC    LOCAL  DEFAULT   11 print_internal
        47: 000000000000111f    17 FUNC    GLOBAL DEFAULT   11 print_1
        48: 0000000000001109    22 FUNC    GLOBAL DEFAULT   11 print

    b,$ objdump --disassemble libhello.so
        00000000000005e1 <print_1>:
        55                       push   %rbp
        48 89 e5                 mov    %rsp,%rbp
        b8 00 00 00 00           mov    $0x0,%eax
        e8 dd ff ff ff           callq  5cc <print>
        c9                       leaveq
        c3                       retq

    c,hello.s
        .globl print_internal
        .hidden print_internal                              #表明属性为hidden
        .type   print_internal, @function

        print_internal:
            .LFB0:
                cfi_startproc
                pushq   %rbp
                .......

        print_1:
            .LFB2:
                .......
                call    print_internal                      #在asm中所有的print symbol都换成了 print_internal
                .......                                     #所以在链接时so内部都是用print_internal链接,而print_internal是hidden(local)的所以不需要plt

        .global print
            .set print, print_internal                      #定义输出global symbol print, 方便so外部的调用
