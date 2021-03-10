0,basic
    a, (gdb) p /x *(int*) (((char*)args[0].reg_save_area)+args[0].gp_offset)                       #int 强转没问题.
            $6 = 0x6cc655aa
        (gdb)

    b,总体看不好使,一般只有arg[0] 是正确的.


1,
    (gdb) break vprintf
        Breakpoint 1 at 0x4005a9: file /usr/include/bits/stdio.h, line 38.
    (gdb) r
        Starting program: /root/noteGit/gnu/gdb/valist/valist

        Breakpoint 1, myfunc (fmt=fmt@entry=0x4006c0 "test 0: %x %lx \n") at main.c:11
    11	    vprintf(fmt, args);
        (gdb) p /x (unsigned long*) (((char*)args[1].reg_save_area)+args[1].gp_offset)              #long 型强转可能会出现地址问题.
        $1 = 0x55aadeaebeffaa54
    (gdb) p /x (unsigned long) (((char*)args[1].reg_save_area)+args[1].gp_offset)
        $2 = 0x55aadeaebeffaa54
    (gdb) p /x (unsigned long ) (((char*)args[1].reg_save_area)+args[1].gp_offset)
        $3 = 0x55aadeaebeffaa54
    (gdb) p /x (unsigned int) (((char*)args[1].reg_save_area)+args[1].gp_offset)
        $4 = 0xbeffaa54
    (gdb) p /x (unsigned int) (((char*)args[0].reg_save_area)+args[0].gp_offset)
        $5 = 0xffffdd18
    (gdb) p /x *(int*) (((char*)args[0].reg_save_area)+args[0].gp_offset)                           #int 强转没问题.
        $6 = 0x6cc655aa
    (gdb)
