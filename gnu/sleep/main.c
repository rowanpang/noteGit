#include <stdio.h>
#include <unistd.h>

int main(int argc,char **argv)
{
	printf("start_\n");
	sleep(1);
	printf("--------start_\n");
	alarm(1);
	return 0;
}
