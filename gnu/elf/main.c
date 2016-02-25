#include <stdio.h>

//char pname[100] __attribute__ ((__section__("psection"),aligned(8))) ="pangwz test" ;
static char pname[40] __attribute__ ((__section__(".psection"))) ="pangwz test" ;
static char ppname[40] __attribute__ ((__section__(".psection"))) ="ppangwz test" ;
static char *pppname __attribute__ ((__section__(".psection"))) ="pppangwz test" ;

int a = 10;
__typeof__(a) b=100;

static char *msg = "pangwzzzzzz";

int main(int argc ,char** argv)
{
	printf("hello world,a:%d,b:%d\n",a,b);
	printf("%s\n",pname);
	pname[2] = '.';
	printf("%s\n",pname);

	return 0;
}

