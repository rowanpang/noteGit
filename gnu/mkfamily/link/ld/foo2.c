#include <stdio.h>

void foo2(void)
{
    printf("call foo1(),from %s\n",__func__);
	foo1();
    printf("hello from %s\n",__func__);
}
