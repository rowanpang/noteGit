#include <stdio.h>
#include "hello_internal.h"

void print(void)
{
	int a,b,c;
	a = b = 10;
	c = a+b;
	return;
}

void print_1(void)
{
    print();
}

hidden_def(print)
