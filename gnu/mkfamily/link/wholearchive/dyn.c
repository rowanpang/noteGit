#include <stdio.h>
#include "msg.h"

void dyn(void)
{
    printf("hello from func %s\n",__func__);

    msg();
}
