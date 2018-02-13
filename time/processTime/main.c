#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<sys/times.h>

int main(int argc,char** argv)
{
    int ret;
    int length=35;
    clock_t in,out;
    struct tms inbuf,outbuf;
    struct timespec tp;
    clockid_t clkid = CLOCK_REALTIME;

    ret = clock_getres(clkid,&tp);
    printf("res sec:nsec,%ld:%ld\n",tp.tv_sec,tp.tv_nsec);


    in = times(&inbuf);
    ret = clock_gettime(clkid,&tp);
    printf("time sec:nsec,%ld:%ld\n",tp.tv_sec,tp.tv_nsec);
    printf("%*s: %ld\n",length,"real cpu time by second-clock()",clock());
    sleep(1);
    usleep(2000);
    printf("hello world,thit task time test\n");
    out = times(&outbuf);
    ret = clock_gettime(clkid,&tp);
    printf("time sec:nsec,%ld:%ld\n",tp.tv_sec,tp.tv_nsec);

    printf("%*s: %ld\n",length,"real cpu time by clock()",clock());
    printf("%*s: %ld\n",length,"real cpu time by times()",in);
    printf("%*s: %ld\n",length,"real cpu time by times()",out);
    printf("%*s: %ld\n",length,"outbuf.tms_utime",outbuf.tms_utime);
    printf("%*s: %ld\n",length,"outbuf.tms_stime",outbuf.tms_stime);
    printf("%*s: %ld\n",length,"user cpu time",(outbuf.tms_utime - inbuf.tms_utime));
    printf("%*s: %ld\n",length,"sys cpu time",(outbuf.tms_stime - inbuf.tms_stime));
    return 0;
}
