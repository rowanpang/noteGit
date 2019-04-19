#include <stdio.h>

void sayHello(void)
{
    printf("hello from func %s\n",__func__);
}
