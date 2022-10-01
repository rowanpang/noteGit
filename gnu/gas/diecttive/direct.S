#define ASM_NL           ;

#ifndef __ALIGN
#define __ALIGN         .align 4,0x90
#define __ALIGN_STR     ".align 4,0x90"
#endif

#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR

#define ENTRY(name) \
        .globl name ASM_NL \
        ALIGN ASM_NL \
        name:

#define ENDPROC(name) \
    .type name, @function ASM_NL \
    END(name)

#define END(name) \
        .size name, .-name

#define NUM_EXCEPTION_VECTORS 20
#define EARLY_IDT_HANDLER_SIZE 9

ENTRY(early_idt_handler_common)
    pushq $0
ENDPROC(early_idt_handler_common)

ENTRY(early_idt_handler_array)
        # 104(%rsp) %rflags
        #  96(%rsp) %cs
        #  88(%rsp) %rip
        #  80(%rsp) error code
        i = 0					#不占用代码地址.
        .rept NUM_EXCEPTION_VECTORS
        pushq $0                # Dummy error code, to make stack frame uniform
        pushq $i                # 72(%rsp) Vector number
        jmp early_idt_handler_common
        i = i + 1
        .fill early_idt_handler_array + i*EARLY_IDT_HANDLER_SIZE - ., 1, 0xcc
	    #目标size - ., 即还需要填充的字节数.
	    #用0xcc(int3)填充.
        .endr
ENDPROC(early_idt_handler_array)


