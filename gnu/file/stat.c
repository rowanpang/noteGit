#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc,char ** argv)
{
	struct stat fst;
	int ret = stat(argv[1],&fst);
	

	printf("stat->st_dev:%#x,major:%d,minor:%d\n",fst.st_dev,major(fst.st_dev),minor(fst.st_dev));
}
