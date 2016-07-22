#include<syslog.h>
#include<stdio.h>

extern void syslog_test();

int main(void)
{

	//syslog(LOG_AUTH|LOG_INFO,"%s","pangwz syslog test");
	syslog_test();
	
	return 0;
}
