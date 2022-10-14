#include <stdio.h>

char foo_msg[128]="Hello world";
int foo_init = 100;
int foo_static = 30;
int foo_uninit;

void show_foo(int isfree)
{
    foo_init = 10;
    foo_uninit = 30;
    foo_static = 20;
    printf("%s,from %s\n", foo_msg,__FILE__);
    return ;
}

