#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

typedef enum ops{
	mapwrite,
	mapread
} ops_t;

int main(int argc,char** argv)
{
    	int rc = 0;
	int lshift = 30;
	int skipshift = 3;
	
	long length;
	long skip;

	int fd=0;
	char *fpath = "/tmp/mmap1G";

	struct stat st;
	char *maddr, *ctmp;
	int i = 0;

	ops_t mop = mapread;

	if (argc == 2)
		lshift = atoi(argv[1]);
	if (argc == 3) {
		lshift = atoi(argv[1]);
		skipshift = atoi(argv[2]);
	}

 	printf("starting pid:%d , lshift:%d , skipshift:%d \n",getpid(),lshift,skipshift);

	length = (1ul << lshift);
	skip = (1ul << skipshift);

	fd = open(fpath,O_RDWR|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
	if (fd < 0) {
	    printf("open file name %s,errno:%d \n", fpath,errno);
	    return -1;
	}
	printf("after open fd:%d\n",fd);

	if (fstat(fd,&st) == -1) {
		perror("fstat");
		rc = -errno;
		goto out;
	}

	if (st.st_size < length) {
		printf("do truncate to length:%#x\n",length);
		if (ftruncate(fd,length) == -1) {
			perror("ftruncate");
			rc = -errno;
			goto out;
		}
		
		mop = mapwrite;
	}
	printf("after truncate.....\n");

	maddr = mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if (maddr == MAP_FAILED) {
	    printf("mmap() failed, errno %d\n", errno);
	    rc = -errno;
	    goto out;
	}

	printf("after mmap,maddr :%p, end:%p\n",maddr,maddr+length);
	
	if (mop == mapwrite) {
		printf("starting valued...\n");
		ctmp = maddr;
		while (ctmp < (maddr + length)) {
			/*
			if (ctmp < (maddr + 0x10))
				printf("now ctmp:%p,skip:%d\n",ctmp,skip);
			*/

			*ctmp = i++;
			ctmp += skip;
			if (ctmp >= (maddr + length)) {
				printf("now ctmp:%p\n",ctmp);
			}

		}
		printf("valued down\n");

	} else {
		printf("starting reading...\n");
		ctmp = maddr;
		while (ctmp < (maddr + length)) {
			i = *ctmp;
			ctmp += skip;
			if (ctmp >= (maddr + length)) {
				printf("now ctmp:%p\n",ctmp);
			}

		}
		printf("reading down\n");
	}

	printf("pausing\n");
	pause();
	rc = munmap(maddr, length);
	if (rc < 0) {
	    printf("munmap() failed, errno %d\n", errno);
	}
out:
	close(fd);
    	return rc;
}
