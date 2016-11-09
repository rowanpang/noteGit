#include<stdio.h>
#include<string.h>

int main(void)
{
	int ret;
	printf("pangwz---test\n");
	printf("pangwz-庞伟振\n");
	
	char *name = "/media/ROWAN/p庞伟振";
	FILE *f1 = fopen(name,"w+");
	char *content = "utf8 src coding program,write to new file,p庞伟振";
	ret = fwrite(content,1,strlen(content),f1);
	printf("result %d, %d\n",__LINE__,ret);

	fclose(f1);	
	/*ret = unlink("庞伟振");*/

	printf("result %d\n",ret);
	return ret;
}
 
