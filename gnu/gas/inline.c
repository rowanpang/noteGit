#include <stdio.h>

char* mystrcpy(char* dest,char* src) 
{
	char *cp = dest;
	while( *cp++ = *src++)
		;

	return dest;
}

int callee(int a,int b,int c,int d,int e,int f,int g,int h,int i,int j,int k,int l)
{
	return a+b+c;
}

int caller(void)
{
	int ret;
	ret = callee(1,2,3,4,5,6,7,8,9,10,11,12);
	ret += 5;
	return ret;
}
int total = 7;
		//"lock addl %0, %1\n\t"
int main(int argc,char **argv)
{
	int a = 3,b;
	char str[20]={0};
	asm("movl %2, %%eax\n\t" 
		"movl %%eax, %0\n\t"
		"lock addl %2, %1\n\t"
		:"=r"(b),"=m"(total)
		:"r"(a)
		:"%eax"
		);
	int c = b;	
	caller();
	printf("b:%d,total:%d\n",b,total);

	mystrcpy(str,"pangwz");

	return 0;
}
