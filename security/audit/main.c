#include<stdio.h>
#include<unistd.h>
#include<libaudit.h>
#include<string.h>
#include<errno.h>

int main(int argc,char *argv[])
{
    int auditfd=0;
    int ret;
    auditfd = audit_open();
    if(auditfd == -1) {
	printf("audit_open error,case:%s\n",strerror(errno));
    }
    
    ret = audit_log_user_message(auditfd,AUDIT_USER_LOGIN,"PANGWZ-AUDIT test",NULL,NULL,NULL,1);
    if(ret <=0) {
	printf("error,case:%s ret:%d\n",strerror(errno),ret);
    } else {
	printf("ret value e.g message sequence: %d\n",ret);
    }	

    ret = audit_log_user_avc_message(auditfd,AUDIT_USER_LOGIN,"byAUDIT test",NULL,NULL,NULL,1);
    if(ret <=0) {
	printf("1-error,case:%s ret:%d\n",strerror(errno),ret);
    } else {
	printf("1-ret value e.g message sequence: %d\n",ret);
    }	

    close(auditfd);

    return 0;
}
