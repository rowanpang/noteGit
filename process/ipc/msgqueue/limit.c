#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1];    /* message data */
};

int main(int argc, char *argv[])
{
    key_t key;
    int msqid;
    struct msgbuf *qbuf;
    int msgsize;
    int total = 0;

    key = 123456;
    /*key=IPC_PRIVATE;*/
    printf("key=%d\n",key);
    msqid = msgget(key, IPC_CREAT|0666);
    if(msqid == -1){
        perror("msgget");
        exit(1);
    }
    printf("msqid=%d\n",msqid);

    /*msgsize = 1024*8 + 2; will cause error,for single msg max 8k*/
    msgsize = 1024*8;
    qbuf =  malloc(msgsize+sizeof(long));
    bzero(qbuf->mtext,msgsize);

    qbuf->mtype = 12;
    printf("Sending a message ...\n");
    printf(" Type: %ld Text: %s\n", qbuf->mtype, qbuf->mtext);
    while(1){
        if((msgsnd(msqid, qbuf,msgsize, 0)) ==-1) {
            perror("msgsnd");
            exit(1);
        }
        total += msgsize;
        printf("snd :%d,total:%d\n", msgsize,total);
        usleep(1000*200);
    }

    return(0);
}
