
#include<stdio.h>

//int pluse(int a unused,int b unused,int c unused)
int pluse(int a UNUSED,int b UNUSED,int c UNUSED)
{
	return a+b;
}

int main(int argc,char ** argv)
{
	int a=atoi(argv[1]);
	int b=atoi(argv[2]);
	printf("pluse value for:%d,%d is %d",a,b,pluse(a,b,1));
	return 0;
}
