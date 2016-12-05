#include <stdio.h>

int value=-1;
	/*value = -1, inc 之后为0，sf = 0;*/
	/*value = -2, inc 之后为-1，sf = 1;*/
int result;

void msg1()
{
	printf("in %s,%d\n",__FUNCTION__,result);
}

void msg2()
{
	printf("in %s,%d\n",__FUNCTION__,result);
}

void incInst()
{
    asm volatile("# beginning \n\t"
			 "movl %1, %%eax\n\t"
             "incl %%eax\n\t"           
             "jns    1f\n "        /*ref value note*/
             "callq msg1\n"
             "1:\n\t"
			 "call msg2\n"		  //!sf just call msg2
             :"=m"(result)
             :"m" (value)
             :"memory");
}

int main(int argc,char **argv)
{
	int a = -1;
	int b = 2;
	int c,d,e;

	e=a+c;
	c=a+b;
	d=a+c;

	incInst();

	return 0;
}

