#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <time.h>

int main(int argc,char **argv)
{
    int ret;
    long int count;
    char vec1[16],vec2[16],vec3[16];
    struct timespec ts,te;
    clockid_t clkid = CLOCK_REALTIME;
    /*clkid = CLOCK_MONOTONIC;*/

    ret = clock_getres(clkid,&ts);


    ret = getrandom(vec1, 16,0);
    ret = getrandom(vec2, 16,0);
    ret = getrandom(vec3, 16,0);
#if defined(__x86_64__)
    count = 0x7fff00;
    if (argc >1 ){
        count = strtol(argv[1],NULL,0);
    }
#elif defined(__powerpc__)
    count = 0x7fff0000;
#endif

    printf("count:%ld/%#lx ,res sec:nsec,%ld:%ld\n",count,count,ts.tv_sec,ts.tv_nsec);

    ret = clock_gettime(clkid,&ts);
    printf("start time sec:nsec,%ld:%ld\n",ts.tv_sec,ts.tv_nsec);

#if defined(__x86_64__)
    asm(    "lea %0,%%r8\n\t"
            "lea %1,%%r9\n\t"
            "lea %3,%%r11\n\t"
            "movups (%%r8),%%xmm1\n\t"
            "movups (%%r9),%%xmm2\n\t"
            "#movups (%%r11),%%xmm3\n\t"
            "movups %%xmm2,%%xmm3\n\t"
            "mov %2,%%r10\n\t"
            "loop:\n\t"
            "xorps %%xmm3,%%xmm2\n\t"
            "aesenc %%xmm1,%%xmm2\n\t"
            "dec %%r10\n\t"
            "movups %%xmm2,%%xmm3\n\t"
            "jne loop\n\t"
        :
        :   "m"(vec1),"m"(vec2),"m"(count),"m"(vec3)
        :   "r8","r9","r10","r11"
        );

#elif defined(__powerpc__)
	#if 0
    asm(    "lvx %0,%%v0\n\t"
            "lvx %1,%%v0\n\t"
            "mov %2,%%r10\n\t"
            "loop: xorps %%xmm0,%%xmm1\n\t"
            "dec %%r10\n\t"
            "jne loop\n\t"
        :
        :   "m"(vec1),"m"(vec2),"m"(count)
        :   "r8","r9"
        );
	#else
		__asm__(
		 	"li %r12,15\n\t"
			"lis %r5,0x7fff\n\t"
			"mfctr %r14\n\t"
			"mtctr %r5\n\t"
			"lvsr %v4,0,%r12\n\t"
			"lvsl %v5,0,%r12\n\t"
			"loop:\n\t"
			"vxor %v4,%v4,%v5\n\t"
			"vcipher %v4,%v4,%v5\n\t"
			"vcipher %v4,%v4,%v5\n\t"
			"bdnz loop\n\t"
			"mtctr %r14\n\t"
		    );
	#endif
#endif

    ret = clock_gettime(clkid,&te);
    printf("end time sec:nsec,%ld:%ld\n",te.tv_sec,te.tv_nsec);

    long ndiff = 1*1000000000 + te.tv_nsec - ts.tv_nsec;
    time_t sdiff = te.tv_sec - 1 - ts.tv_sec + ndiff/1000000000;
    printf("escape %d sec, %ld nsec\n",sdiff,ndiff%1000000000);


    return 0;
}
