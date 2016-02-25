#include<stdio.h>

int main(void)
{
	printf("pangwz---test\n");
	printf("pangwz-庞伟振\n");

	printf("pangwz█\n");
	printf("pangwz%c%c%c\n",0xe2,0x96,0x88);
	printf("pangwz%c%c%c\n",0xe2,0x93,0xb5);

	char in[4]={"庞"};

	printf("%#x ",in[0]);
	printf("%#x ",in[1]);
	printf("%#x \n",in[2]);
	
	char a = 'p';
	printf("%#x \n",a);
	return 0;
}
