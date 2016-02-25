#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>


int main(void)
{
	
	//long int cur_uid = getuid();
	//long int cur_euid = geteuid();
	extern int errno;
	uid_t cur_uid = getuid();
	uid_t cur_euid = geteuid();
	printf("cur_uid:%ld\n",cur_uid);
	printf("cur_euid:%ld\n",cur_euid);
	
	
	if(setgid(0) || setuid(0))
	{
		printf("permisson error,num:%d,str:%s\n",errno,strerror(errno));
		//printf("permisson error,num:%d\n",errno);
		char *msg = strerror(errno);

		printf("error info %s\n",msg);
	}	


	system("cat /proc/self/status");	

	return -1;

}
