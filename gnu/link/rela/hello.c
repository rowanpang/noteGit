#include <stdio.h>

extern char foo_msg[128];
extern int foo_init;
extern int foo_uninit;
extern void show_foo(int isfree);

int hello_init = 20;
int hello_uninit;
static int hello_static = 10;
static int hello_static_uninit;
char msg_hello[128]="msg hello,Hello world";

void show(char* msg)
{
    printf("in func:%s,msg:%s\n",__FILE__,msg);
    return;
}

int main(int argc, char** argv)
{
    static int local_static = 10;
    static int local_static_uninit;

    printf("in func %s\n",__FILE__);

    printf("foo_msg:%s,\n",foo_msg);
    printf("msg_hello:%s\n",msg_hello);

    foo_init = 10;
    foo_uninit = 20;

    hello_init = 40;
    hello_uninit = 30;

    hello_static = 20;
    hello_static_uninit = 20;
    local_static = 20;
    local_static_uninit = 20;

    show(foo_msg);
    show_foo(hello_init);
    return 0;
}
