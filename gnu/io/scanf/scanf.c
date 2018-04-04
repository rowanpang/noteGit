#include <stdio.h>
#include <string.h>

int main(int argc,char ** argv)
{
    printf("sizeof(int):%d\n",sizeof(int));
    printf("sizeof(long):%d\n",sizeof(long));
    long a,b,c;
    float d;
    char str[128];
    goto sscanf;

scanf:
    scanf("%f",&d);
    scanf("%x %d",&a,&b);
    scanf("%d",&c);
    printf("scanf:a:%d,b:%d,c:%d,d:%f\n",a,b,c,d);
    return 0;

sscanf:
    printf("------sscanf---------\n");
    sscanf("10 20 30 40","%d %d",&a,&b);
    sscanf("10 11","%*d %d",&c);
    printf("sscanf 10,20,11:a:%d,b:%d,c:%d\n",a,b,c);

    bzero(str,128);
    sscanf("a, ab, abc, abcd","%c,",str);
    printf("%s\n",str);
    sscanf("#2: <ffffb263cbae38f8>, hrtimer_wakeup, S:01","#%d: <%lx>, %[^,]s,",&a,&b,str);
    printf("a:%d,b:%lx,str:%s",a,b,str);
    return 0;

fscanf:
    printf("------fscanf---------\n");
    FILE* fp=fopen("in.txt","r");
    fscanf(fp,"%d %d",&a,&b);
    fscanf(fp,"%d",&c);
    printf("fscanf:a:%d,b:%d,c:%d\n",a,b,c);
    fclose(fp);
    return 0;

}
