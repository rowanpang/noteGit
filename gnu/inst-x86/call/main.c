#include <stdio.h>

void show2()
{
	printf("msg from show2\n");
}

void show(char* msg)
{
	printf("%s\n",msg);
	show2();
}


char *msg="pagnwz";
int main(int argc,char **argv)
{
	show("msg");
	printf("hello world\n");
	return 0;
}
