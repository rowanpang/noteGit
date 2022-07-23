#include <stdio.h>

extern void show2(char* msg);
void show(char* msg)
{
    printf("in show call show2\n");
    show2(msg);
}
