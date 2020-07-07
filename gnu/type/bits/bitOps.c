#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *include/asm-generic/bitops/__ffs.h
 *linux v3.10.0
 */
static __always_inline unsigned long __ffs(unsigned long word)
{
    int num = 0;

#if BITS_PER_LONG == 64
    if ((word & 0xffffffff) == 0) {
	    num += 32;
	    word >>= 32;
    }
#endif
    if ((word & 0xffff) == 0) {
	    num += 16;
	    word >>= 16;
    }
    if ((word & 0xff) == 0) {
	    num += 8;
	    word >>= 8;
    }
    if ((word & 0xf) == 0) {
	    num += 4;
	    word >>= 4;
    }
    if ((word & 0x3) == 0) {
	    num += 2;
	    word >>= 2;
    }
    if ((word & 0x1) == 0)
	    num += 1;
    return num;
}

int main(int argc, char *argv[])
{
    int i;

    if(argc > 1){
	i = strtol(argv[1],NULL,0);
    }else{
	i = 0x34808;
    }

    /*lsb bit count from 0*/
    printf("%#x,find fist bit set is %d,ffz is %d\n", i,ffs(i),ffs(~i));
    /*lsb bit count from 1*/
    printf("%#x,find fist bit set is %d,ffz is %d\n", i,__ffs(i),__ffs(~i));


    return 0;
}

