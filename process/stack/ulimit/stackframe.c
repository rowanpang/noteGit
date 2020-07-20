#include<stdio.h>
#include<unistd.h>

int hello(void)
{
    char a[2*4096 * 2];
    char b[2*1024*1024];
    printf("hello world\n");

}

int main(int argc,char **argv)
{
	char a[4096 * 2];
	char b[6*1024*1024];

	b[0]=3;
	b[6*1024*1024-1]=5;
	printf("sizeof(a[x]) is %d\n",sizeof(a));
	printf("sizeof(b[x]) is %d\n",sizeof(b));

	hello();

	sleep(1);
	return 0;
}
