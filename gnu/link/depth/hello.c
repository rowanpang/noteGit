#include <stdio.h>

extern void show(char* msg);
void sayhello()
{
    char msg[128]="Hello world";
    printf("in hello call show()\n");
    show(msg);
}
