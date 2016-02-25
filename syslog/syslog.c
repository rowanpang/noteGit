#include<syslog.h>

void syslog_test()
{
	syslog(LOG_AUTH | LOG_INFO,"%s","pangwz------test so");
}
