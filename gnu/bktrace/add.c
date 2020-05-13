#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int add1(int num)
{
	int ret = 0x00;
	int *pTemp = NULL;

	*pTemp = 0x01;  /* 这将导致一个段错误，致使程序崩溃退出 */

	ret = num + *pTemp;

	return ret;
}

int add(int num)
{
	int ret = 0x00;

	ret = add1(num);

	return ret;
}
