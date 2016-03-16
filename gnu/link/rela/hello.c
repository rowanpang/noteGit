#include <stdio.h>
extern char msg[128];
extern void foo_print();
extern void foo_print2();
extern int hh;
int main()
{
    foo_print2();
	hh = 30;
    printf("----%d-------\n",hh);
}
