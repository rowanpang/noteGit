#include <stdlib.h>
#include <stdio.h>

int main(int argc,char **argv)
{
	if(argc < 2){
		printf("Usage:%s [ENV],exp:%s PATH\n",argv[0],argv[0]);
		return -1;	
	}
	printf("env %s:%s\n",argv[1],getenv(argv[1]));
	return 0;
}
