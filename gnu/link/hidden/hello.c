#include <stdio.h>
#include "hello_internal.h"

char msg[128]="Hello world";
void print()
{
    printf("%s\n", msg);
}

void print_1()
{
    print();
}

hidden_def(print)
