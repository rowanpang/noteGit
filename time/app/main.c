#include<stdio.h>
#include<unistd.h>
#include<time.h>

int main(int argc,char** argv)
{
    time_t now = time(NULL);

    printf("utc now from 1970:%ld,%s\n",now,ctime(&now));
    printf("utc:%s\n",asctime(gmtime(&now)));

    printf("zone name:%s-%s\n",tzname[0],tzname[1]);
    printf("zone value in seconds(utc-local):%ld,hours:%ld\n",timezone,timezone/3600);

    printf("local:%s\n",ctime(&now));
    printf("local-2:%s\n",asctime(localtime(&now)));

    return 0;
}
