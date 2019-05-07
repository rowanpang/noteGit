$ ldd dynamiclib
    linux-vdso.so.1 (0x00007ffff7ffd000)
    libhello.so => /root/noteGit/gnu/link/static/libhello.so (0x00007ffff7df7000)
    libc.so.6 => /lib64/libc.so.6 (0x00007ffff79f8000)
    /lib64/ld-linux-x86-64.so.2 (0x0000555555554000)

    连接的是libhello.so

$ ldd static*
    staticelf:
	not a dynamic executable

    staticlib:
	linux-vdso.so.1 (0x00007ffff7ffd000)
	libc.so.6 => /lib64/libc.so.6 (0x00007ffff7bfa000)
	/lib64/ld-linux-x86-64.so.2 (0x0000555555554000)
    staticlib.2:
	linux-vdso.so.1 (0x00007ffff7ffd000)
	libc.so.6 => /lib64/libc.so.6 (0x00007ffff7bfa000)
	/lib64/ld-linux-x86-64.so.2 (0x0000555555554000)

    连接的是libhello.a
