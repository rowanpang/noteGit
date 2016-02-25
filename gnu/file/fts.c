#include <fts.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
	FTS *fts_handle;
	FTSENT *ftsent;
	long unsigned int count = 0;
	if (argc < 2)
	{
		printf("Usage %s path\n",argv[0]);
		return 0;
	}
	const char *namelist[2] = {argv[1], NULL};
	fts_handle = fts_open((char **)namelist,FTS_PHYSICAL | FTS_XDEV,NULL);
	ftsent = fts_read(fts_handle);
	do{
		printf("level:%3d,file:%s\n",ftsent->fts_level,ftsent->fts_path);
		count++;
		if(count % 1000 == 0)
		{
			printf("\n\n\n----------------1000------------------\n\n\n");
		}
	}while((ftsent = fts_read(fts_handle)) != NULL);

	printf("\n\n\n----------------Total:%ld------------------\n\n\n",count);
	return 0;
}
