#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 *下面是子进程的命令处理函数（特定于应用）：
 */
int handle_cmd(int cmd)
{
    if((cmd<0)||(cmd>256)) //suppose child only support 256 commands
    {
	printf("child: invalid command \n");
	return -1;
    }
    printf("child: the cmd from parent is %d\n", cmd);
    return 0;
}

int main(int argc,char* argv[])
{
    int pipeFwCr[2];
    pid_t pid;
    char r_buf[4];
    char* w_buf[256];
    int childexit=0;
    int i;
    int cmd;

    memset(r_buf,0,sizeof(r_buf));

    if(pipe(pipeFwCr)<0) {
        printf("pipe create error\n");
        return -1;
    }

    pid=fork();
    if(pid==0) {
	/*child*/
        printf("child,pid:%d\n",getpid());
        close(pipeFwCr[1]);
        sleep(2);

        while(!childexit)
        {
	    memset(r_buf,0,sizeof(r_buf));
	    /*for cmdSize = 3+1('\0') = 4*/
            read(pipeFwCr[0],r_buf,4);
	    /*printf("child: r_buf:%s\n",r_buf);*/
            cmd=atoi(r_buf);
            if(cmd==0)
            {
		printf("child: receive command from parent \n now child exit\n");
                childexit=1;
            } else if(handle_cmd(cmd)!=0){
                return 0;
	    }
            sleep(1);
        }
        close(pipeFwCr[0]);
        exit(0);
    } else if(pid>0) {
	/*parent*/
	close(pipeFwCr[0]);

	w_buf[0]="003";
	w_buf[1]="005";
	w_buf[2]="777";
	w_buf[3]="000";
	for(i=0;i<4;i++){
	    printf("send cmd:%s\n",w_buf[i]);
	    write(pipeFwCr[1],w_buf[i],4);
	}
	close(pipeFwCr[1]);

	printf("send cmd finished\n");
	wait(NULL);
    }

    return 0;
}
