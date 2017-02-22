#include<stdio.h>
#include<unistd.h>
#include<libaudit.h>
#include<string.h>
#include<errno.h>

int main(int argc,char *argv[])
{

	printf("test for audit log...\n");
	
	int auditfd=0;
	auditfd = audit_open();
	if(auditfd == -1) {
		printf("audit_open error,case:%s\n",strerror(errno));
	}
	
	int ret = audit_log_user_message(auditfd,AUDIT_USER_LOGIN,"PANGWZ-AUDIT test",NULL,NULL,NULL,1);
	if(ret <=0) {
		printf("audit_log error,case:%s\n",strerror(errno));
	} else {
		printf("ret value e.g message sequence: %d\n",ret);
	}	

	close(auditfd);

	return 0;
}
