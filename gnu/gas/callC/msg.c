#include <stdio.h>

extern int func(int arg) asm("_msg");
int msg (int arg)
{
	printf("in %s:%s,%d,argVal:%d\n",__FILE__,__FUNCTION__,__LINE__,arg);
	return 0;
}

