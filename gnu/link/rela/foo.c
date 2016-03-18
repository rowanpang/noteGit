#include <stdio.h>
char msg[128]="Hello world";
int hh = 100;

char msg2[128]="msg2,Hello world";
int hh2 = 100;

static int hh_static = 10;

void foo_print(void)
{
	printf("%s,from %s\n", msg,__FILE__);
	return ;
}

void foo_print2(void)
{
	int hh_local = 0;
	hh2 = 20;
	hh_static = 40;
	hh_local = 50;

	foo_print();

	printf("compatible foo.so.1\n");
	printf("%s,from %s\n", msg2,__FILE__);

	return;
}
