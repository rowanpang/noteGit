#include<stdio.h>
#include<string.h>

int main(void)
{

	unsigned char *fname="sxy.txt";
	FILE* F1=fopen(fname,"w+");
	fclose(F1); 
	
	unsigned char *fname2="庞伟振.txt";
	FILE* F2=fopen(fname2,"w+");
	fclose(F2);

	unsigned char fname3[10]={'p',0xc5,0xd3,'.','t','x','t'};
	FILE* F3=fopen(fname3,"w+");
	fclose(F3);
}
