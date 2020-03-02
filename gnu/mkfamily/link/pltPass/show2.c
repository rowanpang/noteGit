#include <stdio.h>
#include "hiddenMacro.h"
#include "internal.h"

void show2(char* msg)
{
    printf("in func:%s,msg:%s\n", __func__,msg);
}

hidden_def(show2)
