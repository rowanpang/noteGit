0,basic
    a,使用如下命令生成asm文件
        make V=1 -C /lib/modules/`uname -r`/build M=$PWD statickey.s
    b,objdump 的并非不准,而是在模块加载时会根据参数动态确认需要执行的分支.
    c,在编译时或者模块加载时已经判断好了需要跳转的分支,在运行时不需要test/je等条件指令.
    d,ref
        https://www.dazhuanlan.com/2019/10/10/5d9f4b6a20f82/

1, count=0
    a,
        $ insmod ./kmodExample.ko
        $ rmmod kmodExample
    b,dmesg
        [7626170.346575] in func :example_init
        [7626170.346577] hello from file:/root/noteGit/kernel/statickey/statickey.c
        [7626170.346577] hello from file:/root/noteGit/kernel/statickey/statickey.c
        [7626170.346577] end:8738
        [7626170.346578] kernel vermagic:4.11.8-300.fc26.x86_64 SMP mod_unload
        [7626170.346578] module_param int count:0
        [7626170.346578] module_param char *who:who-default
        [7626192.129049] exit in func :example_exit

    d,crash
        crash> dis statickeybranchlikely
            0xffffffffc0bf7073 <statickeybranchlikely>:     nopl   0x0(%rax,%rax,1) [FTRACE NOP]
            0xffffffffc0bf7078 <statickeybranchlikely+5>:   push   %rbp
            0xffffffffc0bf7079 <statickeybranchlikely+6>:   cmp    $0x1,%edi
            0xffffffffc0bf707c <statickeybranchlikely+9>:   mov    %rsp,%rbp
            0xffffffffc0bf707f <statickeybranchlikely+12>:  push   %rbx
            0xffffffffc0bf7080 <statickeybranchlikely+13>:  mov    %edi,%ebx
            0xffffffffc0bf7082 <statickeybranchlikely+15>:  sbb    %edi,%edi
            0xffffffffc0bf7084 <statickeybranchlikely+17>:  not    %edi
            0xffffffffc0bf7086 <statickeybranchlikely+19>:  and    $0x9999,%edi
            0xffffffffc0bf708c <statickeybranchlikely+25>:  nopl   0x0(%rax,%rax,1)                                         #static key 声明为true, 使用*like() 判断,只需添加了nopl.
            0xffffffffc0bf7091 <statickeybranchlikely+30>:  callq  0xffffffffc0bf7000 <say_hello>
            0xffffffffc0bf7096 <statickeybranchlikely+35>:  jmp    0xffffffffc0bf709d <statickeybranchlikely+42>
            0xffffffffc0bf7098 <statickeybranchlikely+37>:  callq  0xffffffffc0bf7020 <myelsemsg>
            0xffffffffc0bf709d <statickeybranchlikely+42>:  test   %ebx,%ebx
            0xffffffffc0bf709f <statickeybranchlikely+44>:  je     0xffffffffc0bf70ad <statickeybranchlikely+58>
            0xffffffffc0bf70a1 <statickeybranchlikely+46>:  mov    $0xffffffffc0bf9030,%rdi
            0xffffffffc0bf70a8 <statickeybranchlikely+53>:  callq  0xffffffffb11c69c0 <static_key_disable>
            0xffffffffc0bf70ad <statickeybranchlikely+58>:  xor    %eax,%eax
            0xffffffffc0bf70af <statickeybranchlikely+60>:  pop    %rbx
            0xffffffffc0bf70b0 <statickeybranchlikely+61>:  pop    %rbp
            0xffffffffc0bf70b1 <statickeybranchlikely+62>:  retq
        crash> dis statickeybranchunlikely
            0xffffffffc0bf703b <statickeybranchunlikely>:   nopl   0x0(%rax,%rax,1) [FTRACE NOP]
            0xffffffffc0bf7040 <statickeybranchunlikely+5>: push   %rbp
            0xffffffffc0bf7041 <statickeybranchunlikely+6>: test   %edi,%edi
            0xffffffffc0bf7043 <statickeybranchunlikely+8>: mov    %rsp,%rbp
            0xffffffffc0bf7046 <statickeybranchunlikely+11>:        je     0xffffffffc0bf704d <statickeybranchunlikely+18>
            0xffffffffc0bf7048 <statickeybranchunlikely+13>:        mov    $0x1111,%edi
            0xffffffffc0bf704d <statickeybranchunlikely+18>:        jmpq   0xffffffffc0bf7059 <statickeybranchunlikely+30>  #static key 声明为true,用 *unlike()判断,所以jmpq到say_hello()
            0xffffffffc0bf7052 <statickeybranchunlikely+23>:        callq  0xffffffffc0bf7020 <myelsemsg>
            0xffffffffc0bf7057 <statickeybranchunlikely+28>:        jmp    0xffffffffc0bf705e <statickeybranchunlikely+35>
            0xffffffffc0bf7059 <statickeybranchunlikely+30>:        callq  0xffffffffc0bf7000 <say_hello>
            0xffffffffc0bf705e <statickeybranchunlikely+35>:        mov    $0x2222,%esi
            0xffffffffc0bf7063 <statickeybranchunlikely+40>:        mov    $0xffffffffc0bf80cb,%rdi
            0xffffffffc0bf706a <statickeybranchunlikely+47>:        callq  0xffffffffb11c7dc1 <printk>
            0xffffffffc0bf706f <statickeybranchunlikely+52>:        xor    %eax,%eax
            0xffffffffc0bf7071 <statickeybranchunlikely+54>:        pop    %rbp
            0xffffffffc0bf7072 <statickeybranchunlikely+55>:        retq
        crash> q


2, count=1
    a,
        $ insmod ./kmodExample.ko count=1
        $ rmmod kmodExample
    b,dmesg
        [7626202.087264] in func :example_init
        [7626202.087265] hello from file:/root/noteGit/kernel/statickey/statickey.c
        [7626202.087278] in else 2:17
        [7626202.087278] end:8738
        [7626202.087279] kernel vermagic:4.11.8-300.fc26.x86_64 SMP mod_unload
        [7626202.087279] module_param int count:1
        [7626202.087279] module_param char *who:who-default
        [7626207.109806] exit in func :example_exit

    c,crash
                     MODULE       NAME                        SIZE  OBJECT FILE
        ffffffffc0c03080  kmodExample                16384  /root/noteGit/kernel/statickey/kmodExample.ko
        crash>  dis statickeybranchunlikely
            0xffffffffc0c0103b <statickeybranchunlikely>:   nopl   0x0(%rax,%rax,1) [FTRACE NOP]
            0xffffffffc0c01040 <statickeybranchunlikely+5>: push   %rbp
            0xffffffffc0c01041 <statickeybranchunlikely+6>: test   %edi,%edi
            0xffffffffc0c01043 <statickeybranchunlikely+8>: mov    %rsp,%rbp
            0xffffffffc0c01046 <statickeybranchunlikely+11>:        je     0xffffffffc0c0104d <statickeybranchunlikely+18>
            0xffffffffc0c01048 <statickeybranchunlikely+13>:        mov    $0x1111,%edi
            0xffffffffc0c0104d <statickeybranchunlikely+18>:        nopl   0x0(%rax,%rax,1)
            0xffffffffc0c01052 <statickeybranchunlikely+23>:        callq  0xffffffffc0c01020 <myelsemsg>
            0xffffffffc0c01057 <statickeybranchunlikely+28>:        jmp    0xffffffffc0c0105e <statickeybranchunlikely+35>
            0xffffffffc0c01059 <statickeybranchunlikely+30>:        callq  0xffffffffc0c01000 <say_hello>
            0xffffffffc0c0105e <statickeybranchunlikely+35>:        mov    $0x2222,%esi
            0xffffffffc0c01063 <statickeybranchunlikely+40>:        mov    $0xffffffffc0c020cb,%rdi
            0xffffffffc0c0106a <statickeybranchunlikely+47>:        callq  0xffffffffb11c7dc1 <printk>
            0xffffffffc0c0106f <statickeybranchunlikely+52>:        xor    %eax,%eax
            0xffffffffc0c01071 <statickeybranchunlikely+54>:        pop    %rbp
            0xffffffffc0c01072 <statickeybranchunlikely+55>:        retq
        crash> q
