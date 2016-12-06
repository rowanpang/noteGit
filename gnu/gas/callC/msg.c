#include <stdio.h>

extern int printf(const char* __fmt,...) asm("printf");
int msg (int arg)
{
	printf("in %s:%s,%d,argVal:%d\n",__FILE__,__FUNCTION__,__LINE__,arg);
	return 0;
}

