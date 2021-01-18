#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>

/*
 *下面是子进程的命令处理函数（特定于应用）：
 */
int handle_cmd(int cmd)
{
    if((cmd<0)||(cmd>256)) {    //suppose child only support 256 commands
	printf("child: invalid command \n");
	return -1;
    }
    printf("child: the cmd from parent is %d\n", cmd);
    return 0;
}

int childproc(int fd)
{
    char r_buf[4];
    int childexit=0;
    int cmd;
    int ret;

    printf("child,pid:%d\n",getpid());

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd,&readfds);

    while(!childexit) {
        memset(r_buf,0,sizeof(r_buf));  /*for cmdSize = 3+1('\0') = 4*/
        ret = select(fd+1,&readfds,NULL,NULL,NULL);
        if(ret == -1){
            perror("select fd error\n");
            goto out;
        }

        if(!FD_ISSET(fd,&readfds)){
            printf("no fds,continue\n");
            continue;
        }

        ret = read(fd,r_buf,4);
        if(ret == -1){
            perror("child: read error\n");
            goto out;
        }else{
            printf("read bytes:%d\n",ret);
        }

        cmd=atoi(r_buf);
        if(cmd==0) {
            printf("child: receive command from parent \n now child exit\n");
            childexit=1;
        } else if((ret =handle_cmd(cmd))!=0 ){
            goto out;
        }
        sleep(1);
    }

    ret = 0;
out:
    close(fd);

    return ret;
}

int parentproc(int fd)
{
    char* w_buf[256];
    int i;

    w_buf[0]="003";
    w_buf[1]="005";
    w_buf[2]="777";
    w_buf[3]="000";
    for(i=0;i<4;i++){
        printf("send cmd:%s\n",w_buf[i]);
        write(fd,w_buf[i],4);
    }
    close(fd);

    printf("send cmd finished\n");
}


int main(int argc,char* argv[])
{
    int pipefd[2];
    pid_t pid;
    int i;

    if(pipe(pipefd)<0) {
        printf("pipe create error\n");
        return -1;
    }

    pid=fork();
    if(pid==0) {                    /*child*/
        close(pipefd[1]);           //close write end, just read.
        childproc(pipefd[0]);
        exit(0);
    } else if(pid>0) {              /*parent*/
	close(pipefd[0]);           //close read end, just write.
        sleep(1);
        parentproc(pipefd[1]);
	wait(NULL);
    }

    return 0;
}
