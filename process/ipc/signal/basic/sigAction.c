#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define PROMPT "Do U want to terminate program?"

char *prompt=PROMPT;

void ctrl_c_op(int signo)
{
    char c;
    write(STDERR_FILENO,prompt,strlen(prompt));
    c = getchar();
    /*manual input 'y'+'Enter' getchar will got the '\n'*/
    while(c != 'y' && c != 'n'){
	write(STDERR_FILENO,prompt,strlen(prompt));
	c = getchar();
    }

    if(c == 'y'){
	exit(0);
    }
}

int  main()
{
    struct sigaction act;

    act.sa_handler=ctrl_c_op;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    if(sigaction(SIGINT,&act,NULL)<0) {
	fprintf(stderr,"Install Signal Action Error:%s\n\a",strerror(errno));
	exit(1);
    }

    while(1);
}




