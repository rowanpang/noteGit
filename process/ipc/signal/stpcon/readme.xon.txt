0,basic
    a,Ctrl+S 并没有直接stop 前台进程,而是对stdio 做了流控.
    b,这样,前台进程在向stdout write时就需要wait,进而间接的被sleep.

1,follow
    a,终端1
        ./sigtstp, 每3s 输出 hello world

    b,终端2
        $ strace -p `pidof sigtstp`
        strace: Process 24901 attached
        restart_syscall(<... resuming interrupted nanosleep ...>) = 0
        write(1, "hello world 4\n", 14)         = 14
        nanosleep({tv_sec=3, tv_nsec=0}, 0x7ffcdc88f910) = 0
        write(1, "hello world 5\n", 14)         = 14
        nanosleep({tv_sec=3, tv_nsec=0}, 0x7ffcdc88f910) = 0
        write(1, "hello world 6\n", 14)         = 14
        nanosleep({tv_sec=3, tv_nsec=0}, 0x7ffcdc88f910) = 0
        write(1, "hello world 7\n", 14)         = 14
        nanosleep({tv_sec=3, tv_nsec=0}, 0x7ffcdc88f910) = 0            #此时在终端1执行 Ctrl+S

        write(1, "hello world 8\n", 14)         = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
                                                                        #程序可以被正常唤醒,并执行write(1,xx)
        --- SIGINT {si_signo=SIGINT, si_code=SI_KERNEL} ---
        +++ killed by SIGINT +++

2,follow
    a,终端1
        ./ixon

    b,终端2
        $ strace -p `pidof ixon`
        strace: Process 24567 attached
        restart_syscall(<... resuming interrupted nanosleep ...>) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0
        nanosleep({tv_sec=2, tv_nsec=0}, 0x7ffc760c9d20) = 0


    c,终端1上执行Ctrl+S, 并不影响ixon进程的执行.
