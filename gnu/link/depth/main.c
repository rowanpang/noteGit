#include <stdio.h>

extern void sayhello();
extern void show(char* msg);
extern void show2(char* msg);

int main()
{
    sayhello();
    show("aaaaaaaa");
    show2("--------");
}
