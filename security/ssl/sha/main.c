#include <stdio.h>
#include <errno.h>
#include <openssl/sha.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int arch,char** argv)
{
	int fd;
	int size;	
	unsigned char digest[20]={0};
	char *dr=NULL;
	char *buf =NULL; 
	int ret=0;

	char *filepath=argv[1];

	fd=open(filepath,O_RDONLY);
	if(fd < 0){
		perror("when open");
	}
	size=lseek(fd,0,SEEK_END);
	buf=malloc(size);
	if(buf == NULL){
		perror("malloc");
		return -1;
	}
	lseek(fd,0,SEEK_SET);
	ret = read(fd,buf,size);
	if(ret != size){
		perror("when read");
	}
	printf("size %d read\n",size);

	SHA1(buf,size,digest);
	int i;	

	for(i=0;i<20;i++){
		printf("%.2x",digest[i]);
	}
	printf("	%s\n",filepath);

	return 0;
}
	
	
	
	
		
