#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<sys/times.h>
#include<sys/time.h>

int main(int argc,char** argv)
{
    int i;
    struct timeval tv;
    for (i = 0; i < 4; ++i) {
	gettimeofday(&tv, NULL);
	printf("%d\t%d\n", tv.tv_usec, tv.tv_sec);
	printf("%.2f\t%d\n", tv.tv_usec/1000000.0, tv.tv_sec);
	sleep(1);
    }
    return 0;
}
