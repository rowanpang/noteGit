#include<stdio.h>
#include<unistd.h>
#include<time.h>


#define NS_PER_SEC 1000000000
void nowtime_ns()
{
    printf("---------------------------struct timespec---------------------------------------\n");
    printf("[time(NULL)]     :     %ld\n", time(NULL));
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("clock_gettime : tv_sec=%ld, tv_nsec=%ld\n", ts.tv_sec, ts.tv_nsec);

    struct tm t;
    char date_time[64];
    strftime(date_time, sizeof(date_time), "%Y-%m-%d %H:%M:%S", localtime_r(&ts.tv_sec, &t));
    printf("clock_gettime : date_time=%s, tv_nsec=%ld\n", date_time, ts.tv_nsec);
}

void monictime_ns()
{
    printf("---------------------------struct timespec----CLOCK_MONOTONIC------------------------------\n");
    printf("[time(NULL)]     :     %ld\n", time(NULL));
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("clock_gettime : tv_sec=%ld, tv_nsec=%ld\n", ts.tv_sec, ts.tv_nsec);

}

int main(int argc,char** argv)
{
    time_t now = time(NULL);
    int length=25;

    struct timespec start,end;
    long t;

    printf("%*s: %ld\n",length,"utc second since 1970",now);
    printf("%*s: %s",length,"gmtime/utc string",asctime(gmtime(&now)));

    printf("%*s: %s-%s\n",length,"zone name",tzname[0],tzname[1]);

    printf("%*s: %ld\n",length,"zone value in seconds",timezone);
    printf("%*s: %ld\n",length,"zone value in hour",timezone/(60*60));

    printf("%*s: %ld\n",length,"local time(second)",(now - timezone));
    printf("%*s: %ld\n",length,"local time(second)",mktime(localtime(&now)));
    printf("%*s: %s",length,"local time(string)",ctime(&now));
    printf("%*s: %s",length,"local time(string2)",asctime(localtime(&now)));


    nowtime_ns();
    monictime_ns();

    clock_gettime(CLOCK_MONOTONIC,&start);
    clock_gettime(CLOCK_MONOTONIC,&end);

    t=(end.tv_sec-start.tv_sec)*NS_PER_SEC + end.tv_nsec - start.tv_nsec;
    printf("escape: %ld.%09ld sec\n",t/NS_PER_SEC, t%NS_PER_SEC);

    return 0;
}
