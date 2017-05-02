#include<stdio.h>
#include<unistd.h>
#include<time.h>

int main(int argc,char** argv)
{
    time_t now = time(NULL);
    int length=25;

    printf("%*s: %ld\n",length,"utc second since 1970",now);
    printf("%*s: %s",length,"gmtime/utc string",asctime(gmtime(&now)));

    printf("%*s: %s-%s\n",length,"zone name",tzname[0],tzname[1]);

    printf("%*s: %ld\n",length,"zone value in seconds",timezone);
    printf("%*s: %ld\n",length,"zone value in hour",timezone/(60*60));

    printf("%*s: %ld\n",length,"local time(second)",(now - timezone));
    printf("%*s: %ld\n",length,"local time(second)",mktime(localtime(&now)));
    printf("%*s: %s",length,"local time(string)",ctime(&now));
    printf("%*s: %s",length,"local time(string2)",asctime(localtime(&now)));

    return 0;
}
