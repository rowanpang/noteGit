#include<syslog.h>

void syslog_test()
{
	syslog(LOG_AUTHPRIV | LOG_INFO,"%s","pangwz------test so");
}
