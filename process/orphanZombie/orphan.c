#include<stdio.h>
#include<unistd.h>
#include<errno.h>

int doChild()
{
    int i;
    char buf[20];
    for (i = 0; i < 5; ++i) {
	printf("Im child\n");
	sleep(1);
    }
    i = read(0,buf,10);
    /*
     *will case EIO error,for bg process read terminal
     */
    if(i == -1){
	perror("read error");
    }else{
	printf("read %d byts\n",i);
    }
    sleep(30);
}

int main(int argc,char** argv)
{
    int cpid;

    cpid = fork();
    if (cpid == 0){
	/*child*/
	doChild();
    }else if (cpid > 0){
	/*parent*/
	printf("Im parent ppid:%d,cpid:%d\n",getpid(),cpid);
	sleep(3);
	printf("--------orphan child----------\n");
    }else{
	perror("when do fork");
    }


    /*will be here twice , each for father and child process*/
    printf("-----process exit pid:%d\n",getpid());

    return 0;
}

