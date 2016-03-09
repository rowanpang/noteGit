#include <stdio.h>
char msg[128]="Hello world";
int hh = 100;
void foo_print()
{
	printf("%s,from %s\n", msg,__FILE__);
}
