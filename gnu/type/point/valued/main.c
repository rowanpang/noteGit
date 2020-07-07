#include <stdio.h>

int main()
{
	int ret = 0;

	int a;
	int *pa;

	pa = &a;
	a = 10;
	printf("a:%d,*pa:%d\n",a,*pa);
	*pa = 20;
	printf("a:%d,*pa:%d\n",a,*pa);

	return ret;
}
