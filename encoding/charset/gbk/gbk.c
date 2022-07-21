#include<stdio.h>
#include<string.h>

int main(void)
{
	int ret;
	printf("pangwz---test\n");
	printf("pangwz-≈”Œ∞’Ò\n");

	char *name = "niGBK≈”";
	FILE *f1 = fopen(name,"a+");
	char *content = "gkb src coding program,write to new file,p≈”Œ∞’Ò\n";
        fprintf(f1,"%s",content);
	ret = fwrite(content,1,strlen(content),f1);
	printf("result %d, %d\n",__LINE__,ret);

	fclose(f1);
	/*ret = unlink("≈”Œ∞’Ò");*/

	printf("result %d\n",ret);
	return ret;
}

