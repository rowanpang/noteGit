#include <stdio.h>
#include "hiddenMacro.h"

void show2(char* msg);
void show2(char* msg)
{
    printf("in func:%s,msg:%s\n", __func__,msg);
}
