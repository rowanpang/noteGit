#include<stdio.h>
#include<unistd.h>
#include<time.h>

#define NS_PER_SEC 1000000000
int main(int argc,char** argv)
{
    int i,j,k;
    struct timespec start,end;
    long t;

    clock_gettime(CLOCK_MONOTONIC,&start);
    for (i=0; i<100; i++) {
        for (j=0; j<1000; j++) {
            for (k=0; k<10000; k++) {
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&end);
    t=(end.tv_sec-start.tv_sec)*NS_PER_SEC + end.tv_nsec - start.tv_nsec;
    printf("escape 1: %ld.%09ld sec\n",t/NS_PER_SEC, t%NS_PER_SEC);

    clock_gettime(CLOCK_MONOTONIC,&start);
    for (i=0; i<10000; i++) {
        for (j=0; j<1000; j++) {
            for (k=0; k<100; k++) {
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&end);
    t=(end.tv_sec-start.tv_sec)*NS_PER_SEC + end.tv_nsec - start.tv_nsec;
    printf("escape 2: %ld.%09ld sec\n",t/NS_PER_SEC, t%NS_PER_SEC);
    return 0;
}
