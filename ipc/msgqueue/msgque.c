#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_SEND_SIZE 80

struct mymsgbuf {
    long mtype;
    char mtext[MAX_SEND_SIZE];
};

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text)
{
    printf("msgqueue_id=%d\n",qid);
    /* Send a message to the queue */
    printf("Sending a message ...\n");
    qbuf->mtype = type;
    strcpy(qbuf->mtext, text);
    printf(" Type: %ld Text: %s\n", qbuf->mtype, qbuf->mtext);

    if((msgsnd(qid, (struct msgbuf *)qbuf, strlen(qbuf->mtext)+1, 0)) ==-1) {
        perror("msgsnd");
        exit(1);
    }
}

void read_message(int qid, struct mymsgbuf *qbuf, long type)
{
    /* Read a message from the queue */
    printf("Reading a message ...\n");
    qbuf->mtype = type;
    msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0);
    printf(" Type: %ld Text: %s\n", qbuf->mtype, qbuf->mtext);
}

void remove_queue(int qid)
{
    /* Remove the queue */
    msgctl(qid, IPC_RMID, 0);
}

void change_queue_mode(int qid, char *mode)
{
    struct msqid_ds myqueue_ds;
    /* Get current info */
    msgctl(qid, IPC_STAT, &myqueue_ds);

    /* Convert and load the mode */
    sscanf(mode, "%ho", &myqueue_ds.msg_perm.mode);

    /* Update the mode */
    msgctl(qid, IPC_SET, &myqueue_ds);
}

void usage(void)
{
    fprintf(stderr, "msgtool - A utility for tinkering with msg queues\n");
    fprintf(stderr, "\nUSAGE: msgtool (s)end <type> <messagetext> <msgid>\n");
    fprintf(stderr, " (r)ecv <type>\n");
    fprintf(stderr, " (d)elete\n");
    fprintf(stderr, " (m)ode <octal mode>\n");
    fprintf(stderr, "note: type must be number!\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    key_t key;
    int msgqueue_id;
    struct mymsgbuf qbuf;

    if(argc == 1)
        usage();

    /* Create unique key via call to ftok() */
    /* key = ftok(".", 'w'); */
    key = 123456;
    /*key=IPC_PRIVATE;*/
    printf("key=%d\n",key);
    /* Open the queue - create if necessary */
    if((tolower(argv[1][0]))!='s') {
        if((msgqueue_id = msgget(key, IPC_CREAT|0666)) == -1) {
            perror("msgget");
            exit(1);
        }
        printf("msgqueue_id=%d\n",msgqueue_id);
    }

    switch(tolower(argv[1][0])) {
        case 's':
            send_message(atoi(argv[4]), (struct mymsgbuf *)&qbuf, atol(argv[2]), argv[3]);
            break;
        case 'r':
            read_message(msgqueue_id, &qbuf, atol(argv[2]));
            break;
        case 'd':
            remove_queue(atoi(argv[2]));
            remove_queue(msgqueue_id);
            break;
        case 'm':
            change_queue_mode(msgqueue_id, argv[2]);
            break;
        default:
            usage();
    }

    return(0);
}
