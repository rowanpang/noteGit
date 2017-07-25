#include <stdio.h>
#include "hiddenMacro.h"

void show(char* msg);
void show(char* msg)
{
    printf("in func:%s,msg:%s\n",__func__, msg);
}
