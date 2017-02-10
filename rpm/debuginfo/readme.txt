1,gdb
    1),gdb hello
    2),symbol-file hello.debug
    3),run
    4),finish

2,eu-strip add .gnu_debuglink to elf exp:
    objdump --full-contents --section=.gnu_debuglink hello
    hello:     file format elf64-x86-64
    Contents of section .gnu_debuglink:
     0000 68656c6c 6f2e6465 62756700 f76d395c  hello.debug..m9\

3,gdb 会自动找同目录下的hello.debug
