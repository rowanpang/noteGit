#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<sys/times.h>

int main(int argc,char** argv)
{
    int length=25;
    clock_t in,out;
    struct tms inbuf,outbuf;

    in = times(&inbuf);
    sleep(1);
    printf("hello world,thit task time test\n");
    out = times(&outbuf);

    printf("%*s: %ld\n",length,"real cpu time by second-clock()",clock());
    printf("%*s: %ld\n",length,"real cpu time by second-times()",in);
    printf("%*s: %ld\n",length,"real cpu time by second-times()",out);
    printf("%*s: %ld\n",length,"user cpu time",(outbuf.tms_utime - inbuf.tms_utime));
    printf("%*s: %ld\n",length,"sys cpu time",(outbuf.tms_stime - inbuf.tms_stime));
    return 0;
}
