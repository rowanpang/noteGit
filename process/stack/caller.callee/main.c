#include <stdio.h>
#include <stdlib.h>

int myadd(int a, int b)
{
	int c;
	int d;

	c = random();

	d = a + b + c;
	return d;
}

int main()
{
	int a;
	int b;

	int sum,sum1;

	a = random();
	b = random();

	sum = a + b;

	sum1 = myadd(a,b);

	return 0;
}


