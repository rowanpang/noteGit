#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <string.h>


int main(int argc,char **argv)
{
	int ret = 0;
	int fd = 0;
	char buf[128] = {0};
	fd=ret=open("/dev/i2c-0",O_RDWR);
	if(ret == -1){
		perror("open");
		goto RET;
	}
	if(argc < 2){
		ret=write(fd,"pangwz",7);
	}else{
		ret=write(fd,argv[1],strlen(argv[1])+1);
	}
	printf("after write ret:%d\n",ret);

	ret=read(fd,buf,10);
	printf("after read ret:%d,%s\n",ret,buf);

RET:
	return ret;
}
