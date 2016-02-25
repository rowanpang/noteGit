#include <stdio.h>
#include "hello_internal.h"
char msg[128]="Hello world";
void print()
{
    printf("%s\n", msg);
}

//extern void print_hidden() __attribute ((visibility("hidden")));
extern void print_hidden();
extern __typeof__(print_hidden) print_hidden __asm__("print_hidden_internal") __attribute ((visibility("hidden")));
//extern __typeof__(print_hidden) print_hidden "print_hidden_internal" __attribute ((visibility("hidden")));

void print_hidden()
{
    printf("%s,%s\n", __func__,msg);
}
//__asm__(".global  print_hidden \n.set print_hidden ,print_hidden_internal");

void print_1()
{
    print();
	print_hidden();
}

hidden_def(print)
