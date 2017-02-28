/*********************************************************************************************
#####         �Ϻ�Ƕ��ʽ��԰-�������̳�         #####
#####                            www.embedclub.com                               #####
#####                     http://embedclub.taobao.com                       #####

* File��		memdev_test.c
* Author:		Hanson
* Desc��	virual memory  device driver test code
* History:	May 20th 2011
*********************************************************************************************/
#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp0 = NULL;
	char Buf[4096];
	int result;
	
	/*��ʼ��Buf*/
	strcpy(Buf,"Mem is char dev!");
	printf("BUF: %s\n",Buf);
	
	/*���豸�ļ�*/
	fp0 = fopen("/dev/memdev0","r+");
	if (fp0 == NULL)
	{
		perror("Open Memdev0 Error!\n");
		return -1;
	}
	
	/*д���豸*/
	result = fwrite(Buf, sizeof(Buf), 1, fp0);
	if (result  == -1)
	{
		perror("fwrite Error!\n");
		return -1;
	}
	
	/*���¶�λ�ļ�λ�ã�˼��û�и�ָ����кκ��)*/
	fseek(fp0,0,SEEK_SET);
	
	/*���Buf*/
	strcpy(Buf,"Buf is NULL!");
	printf("BUF: %s\n",Buf);
	
	sleep(1);
	/*�����豸*/
	result = fread(Buf, sizeof(Buf), 1, fp0);
	if (result  == -1)
	{
		perror("fread Error!\n");
		return -1;
	}
	
	/*�����*/
	printf("BUF: %s\n",Buf);
	
	return 0;	

}
