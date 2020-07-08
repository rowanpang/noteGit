#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char** argv)
{
    char* pwd = "askdf";
    /*if (!strcmp("asdf","abcd")){ this format will preprocess,so always in-correct*/
    if (!strcmp(pwd,"abcd")){
	printf("correct\n");
    }else{
	printf("in-correct\n");
    }

    printf("hello world,pid:%d\n",getpid());

    /*sleep(300);*/

    return 0;
}
