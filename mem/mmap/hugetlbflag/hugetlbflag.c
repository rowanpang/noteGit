// SPDX-License-Identifier: GPL-2.0
/*
 * Example of using hugepage memory in a user application using the mmap
 * system call with MAP_HUGETLB flag.  Before running this program make
 * sure the administrator has allocated enough default sized huge pages
 * to cover the 256 MB allocation.
 *
 * For ia64 architecture, Linux kernel reserves Region number 4 for hugepages.
 * That means the addresses starting with 0x800000... will need to be
 * specified.  Specifying a fixed address is not required on ppc64, i386
 * or x86_64.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define LENGTH (256UL*1024*1024)
#define PROTECTION (PROT_READ | PROT_WRITE)

#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0x40000 /* arch specific */
#endif

/* Only ia64 requires this */
#ifdef __ia64__
#define ADDR (void *)(0x8000000000000000UL)
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_FIXED)
#else
#define ADDR (void *)(0x0UL)
#define FLAGS (MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB)
#endif

static void check_bytes(char *addr)
{
	printf("First hex is %x\n", *((unsigned int *)addr));
}

static void write_bytes(char *addr)
{
	unsigned long i;

	for (i = 0; i < LENGTH; i++)
		*(addr + i) = (char)i;
}

static int read_bytes(char *addr)
{
	unsigned long i;

	check_bytes(addr);
	for (i = 0; i < LENGTH; i++)
		if (*(addr + i) != (char)i) {
			printf("Mismatch at %lu\n", i);
			return 1;
		}
	return 0;
}

int main(void)
{
	void *addr;
	int ret;

	//addr = mmap(ADDR, LENGTH, PROTECTION, FLAGS, -1, 0);
	//addr = mmap(ADDR, LENGTH, PROTECTION | PROT_EXEC, FLAGS | MAP_FIXED, -1, 0);
        /*
         * MAP_FIXED,SIGSEGV:
         * $ uname -a
         *      Linux rowanInspur 5.8.15-301.fc33.x86_64 #1 SMP Thu Oct 15 16:58:06 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
         *      --- SIGSEGV {si_signo=SIGSEGV, si_code=SEGV_MAPERR, si_addr=0x10000000} ---
         */
	addr = mmap(ADDR, LENGTH, PROTECTION | PROT_EXEC, FLAGS , -1, 0);

	if (addr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	printf("Returned address is %p\n", addr);
	check_bytes(addr);
	write_bytes(addr);
	ret = read_bytes(addr);

	/* munmap() length of MAP_HUGETLB memory must be hugepage aligned */
	if (munmap(addr, LENGTH)) {
		perror("munmap");
		exit(1);
	}

	return ret;
}
