#include <stdio.h>
char msg[128]="Hello world";
int hh = 100;

void foo_print()
{
	printf("%s,from %s\n", msg,__FILE__);
	printf("compatible for foo.so.1\n");
}

char msg2[128]="msg2,Hello world";
int hh2 = 100;
void foo_print2()
{
	printf("compatible foo.so.1 ");
	printf("%s,from %s\n", msg2,__FILE__);
}
