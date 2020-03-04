
#include <stdio.h>

void msg(void)
{
    printf("hello from func %s\n",__func__);
}

void msg_direct(void)
{
    printf("hello from func %s\n",__func__);
}
