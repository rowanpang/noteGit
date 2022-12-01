$ ldd static
        not a dynamic executable

$ readelf --wide --program-headers ./static                                 #静态链接不需要ld..
    Elf file type is EXEC (Executable file)
    Entry point 0x401cf0
    There are 8 program headers, starting at offset 64

    Program Headers:
      Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
      LOAD           0x000000 0x0000000000400000 0x0000000000400000 0x000488 0x000488 R   0x1000
      LOAD           0x001000 0x0000000000401000 0x0000000000401000 0x081ddd 0x081ddd R E 0x1000
      LOAD           0x083000 0x0000000000483000 0x0000000000483000 0x02704b 0x02704b R   0x1000
      LOAD           0x0ab000 0x00000000004ac000 0x00000000004ac000 0x005310 0x006b40 RW  0x1000
      NOTE           0x000200 0x0000000000400200 0x0000000000400200 0x000044 0x000044 R   0x4
      TLS            0x0ab000 0x00000000004ac000 0x00000000004ac000 0x000020 0x000060 R   0x8
      GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x10
      GNU_RELRO      0x0ab000 0x00000000004ac000 0x00000000004ac000 0x003000 0x003000 R   0x1

     Section to Segment mapping:
      Segment Sections...
       00     .note.gnu.build-id .note.ABI-tag .rela.plt
       01     .init .plt .text __libc_freeres_fn .fini
       02     .rodata .stapsdt.base .eh_frame .gcc_except_table
       03     .tdata .init_array .fini_array .data.rel.ro .got .got.plt .data __libc_subfreeres __libc_IO_vtables __libc_atexit .bss __libc_freeres_ptrs
       04     .note.gnu.build-id .note.ABI-tag
       05     .tdata .tbss
       06
       07     .tdata .init_array .fini_array .data.rel.ro .got

$ readelf --wide --program-headers /usr/bin/ls
    Elf file type is DYN (Shared object file)
    Entry point 0x6b60
    There are 13 program headers, starting at offset 64

    Program Headers:
      Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
      PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0002d8 0x0002d8 R   0x8
      INTERP         0x000318 0x0000000000000318 0x0000000000000318 0x00001c 0x00001c R   0x1
          [Requesting program interpreter: /lib64/ld-linux-x86-64.so.2]
      LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x0034e8 0x0034e8 R   0x1000
      LOAD           0x004000 0x0000000000004000 0x0000000000004000 0x0130d1 0x0130d1 R E 0x1000
      LOAD           0x018000 0x0000000000018000 0x0000000000018000 0x007918 0x007918 R   0x1000
      LOAD           0x01ff70 0x0000000000020f70 0x0000000000020f70 0x001308 0x0025d0 RW  0x1000
      DYNAMIC        0x020a18 0x0000000000021a18 0x0000000000021a18 0x000210 0x000210 RW  0x8
      NOTE           0x000338 0x0000000000000338 0x0000000000000338 0x000030 0x000030 R   0x8
      NOTE           0x000368 0x0000000000000368 0x0000000000000368 0x000044 0x000044 R   0x4
      GNU_PROPERTY   0x000338 0x0000000000000338 0x0000000000000338 0x000030 0x000030 R   0x8
      GNU_EH_FRAME   0x01d1e8 0x000000000001d1e8 0x000000000001d1e8 0x00057c 0x00057c R   0x4
      GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x10
      GNU_RELRO      0x01ff70 0x0000000000020f70 0x0000000000020f70 0x001090 0x001090 R   0x1

     Section to Segment mapping:
      Segment Sections...
       00
       01     .interp
       02     .interp .note.gnu.property .note.gnu.build-id .note.ABI-tag .gnu.hash .dynsym .dynstr .gnu.version .gnu.version_r .rela.dyn .rela.plt
       03     .init .plt .plt.sec .text .fini
       04     .rodata .eh_frame_hdr .eh_frame
       05     .init_array .fini_array .data.rel.ro .dynamic .got .data .bss
       06     .dynamic
       07     .note.gnu.property
       08     .note.gnu.build-id .note.ABI-tag
       09     .note.gnu.property
       10     .eh_frame_hdr
       11
       12     .init_array .fini_array .data.rel.ro .dynamic .got
$
