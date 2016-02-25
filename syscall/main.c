#include<stdio.h>
#include<unistd.h>
#include<syscall.h>


void main()
{
	printf("system call test:\n");

	printf("getpid-glibc:%d\n",getpid());

	printf("getpid-syscall:%d\n",syscall(__NR_getpid));

}
