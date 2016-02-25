#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>

int main(void)
{
	FILE *fh = NULL;
	fh = fopen("./test_fopen.txt","r");
	
	if(fh == NULL)
	{
		printf("fopen error %d,%s\n",errno,strerror(errno));
	}
	else
	{
		printf("fopen ok \n");
		fclose(fh);
	}


	int fd = 0;
	fd = open("./test_open.txt",O_RDONLY);
	
	if(fd == -1)
	{
		printf("open error \n");
	}	
	else
	{
		printf("open ok \n");
		close(fd);	
	}

	return 0;
}
