#include<stdio.h>
#include<unistd.h>

int main(int argc,char **argv)
{
	int a[4096 * 2];

	printf("sizeof(a[x]) is %d\n",sizeof(a));
	sleep(30);

	return 0;
}
