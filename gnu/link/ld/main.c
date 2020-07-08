#include <stdio.h>

extern void foo1(void);
extern void foo2(void);

int main(int argc,char** argv)
{
    foo1();
    foo2();

    return 0;
}
