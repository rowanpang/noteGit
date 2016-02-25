#include <stdio.h>

int main(int argc,char ** argv)
{
    printf("sizeof(int):%d\n",sizeof(int));
    int a,b,c;
    float d;
    goto scanf;

scanf:
    scanf("%f",&d);
    scanf("%x %d",&a,&b);
    scanf("%d",&c);
    printf("scanf:a:%d,b:%d,c:%d,d:%f\n",a,b,c,d);
    return 0;

sscanf:
    printf("------sscanf---------\n");
    sscanf("10 20 30 40","%d %d",&a,&b);
    sscanf("10","%d",&c);
    printf("sscanf:a:%d,b:%d,c:%d\n",a,b,c);
    return 0;

fscanf:
    printf("------fscanf---------\n");
    FILE* fp=fopen("in.txt","r");
    fscanf(fp,"%d %d",&a,&b);
    fscanf(fp,"%d",&c);
    printf("fscanf:a:%d,b:%d,c:%d\n",a,b,c);
    close(fp);
    return 0;

}
