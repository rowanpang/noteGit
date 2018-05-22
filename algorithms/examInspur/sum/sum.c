#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int c,char** argv)
{
	unsigned long numCount = 0;
	unsigned long tmp = 0;
	unsigned long sum = 0;

	FILE* infp=fopen("sum.in","r");
	size_t macInitSize = 10;
	size_t mactmpSize = macInitSize;
	char* lineptr = malloc(mactmpSize);

	int ret = 0;

	printf("---------line1---------\n");
	ret = getline(&lineptr,&mactmpSize,infp);
	printf("ret:%d\n",ret);
	sscanf(lineptr,"%d",&numCount);
	printf("numCount:%d\n",numCount);
	
	memset(lineptr,0,mactmpSize);
	printf("---------line2---------\n");
	ret = getline(&lineptr,&mactmpSize,infp);
	printf("ret:%d,mactmpSize:%d\n",ret,mactmpSize);
	printf("lineptr:%s\n",lineptr);
	char* startp=lineptr;
	char* endp=lineptr;
	int i = 0;
	while(*startp && i<numCount)
	{
		i++;
		while(*endp && ((*endp == ' ')||(*endp == '\t')))
			endp++;
		if(!*endp || (*endp =='\n'))
			break;
		startp = endp;

		while(*endp && *endp != ' ' && *endp != '\t')
			endp++;
		*endp = '\0';
		sscanf(startp,"%d",&tmp);
		sum += tmp;
		printf("----i:%d,tmp:%d,sum:%d------\n",i,tmp,sum);
		tmp = 0;
		endp++;
	}

	FILE* outfp=fopen("sum.out","w+");
	fprintf(outfp,"%ld",sum);

	fclose(infp);
	fclose(outfp);
	free(lineptr);

	return 0;
}
