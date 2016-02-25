#include<stdio.h>
#include<fcntl.h>

void main()
{
	int um = umask(0022);
	umask(um);
	printf("um value %#.3o\n",um);
	creat("./ct_bymain",0777);

}

