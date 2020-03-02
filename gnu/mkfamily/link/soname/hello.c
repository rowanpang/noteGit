#include <stdio.h>
extern char msg[128];
extern void foo_print();
extern int hh;
int main()
{
    foo_print();
    printf("----%d-------\n",hh);
}
