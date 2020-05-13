#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>	    /* for signal */
#include <execinfo.h> 	/* for backtrace() */

extern void dump(void);
extern void signal_handler(int signo);
extern int add(int num);

int main(int argc, char *argv[])
{
	int sum = 0x00;

	signal(SIGSEGV, signal_handler);  /* 为SIGSEGV信号安装新的处理函数 */

	sum = add(sum);

	printf(" sum = %d \n", sum);

	return 0x00;
}
