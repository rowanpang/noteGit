#include<stdio.h>
#include<unistd.h>

int main(int argc,char **argv)
{
	char a[4096 * 2];
	char b[9*1024*1024];

	b[0]=3;
	b[9*1024*1024-1]=5;
	printf("sizeof(a[x]) is %d\n",sizeof(a));
	printf("sizeof(b[x]) is %d\n",sizeof(b));
	sleep(1);

	return 0;
}
