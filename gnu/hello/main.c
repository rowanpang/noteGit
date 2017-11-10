#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void show(char* msg)
{
    printf("%s\n",msg);
}

int argTest(int a[3]){
    a[1] = 1;
    return 1;
}

char *msg="pagnwz";
int main(int argc,char **argv)
{
    int a = 9;
    int b = -1;
    int c = a - b;
    int d = 3- (-1);
    int sec=0;
    printf("hello world\n");
    show("msg");

    int test=4;
    int aa[3] = {1,test,5};
    argTest(aa);
    printf("before:%d,after:%d",test,aa[1]);


    if (argc > 1){
        sec = atoi(argv[1]);
        printf("sleep %ds\n",sec);
        sleep(sec);
    }
    return 0;
}
