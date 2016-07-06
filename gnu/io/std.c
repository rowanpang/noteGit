#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//open 会使用当前最小的fd.
int main(int argc,char **argv)
{
	int fd0,fd1,fd2,fd3,fd4;

	fd3 = open("./fd3",O_RDWR|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
	fd4 = open("./fd4",O_RDWR|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
	close(0);
	close(1);
	close(2);
	fd0 = open("./fd0",O_RDWR|O_CREAT|O_APPEND,00600);
	fd1 = open("./fd1",O_RDWR|O_CREAT|O_APPEND,00600);
	fd2 = open("./fd2",O_RDWR|O_CREAT|O_APPEND,00600);

	printf("0~4:%d,%d,%d,%d,%d\n",fd0,fd1,fd2,fd3,fd4);
	
	return 0;
}
