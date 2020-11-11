#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int fileread(void)
{
    FILE *fp = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    fp = fopen("./test.txt","r");
    if(fp == NULL){
        perror("\033[0;31mwhen open test.txt");
        printf("\033[0m");
        exit(EXIT_FAILURE);
    }

    while((read = getline(&line,&len,fp)) != -1){
        line[read-1] = '\0';
        printf("read:%s,len:%zu\n",line,read);
    }

    if(line)
        free(line);

    if(fp)
        fclose(fp);
    return 0;
}

int capdac()
{
    int fd;
    char *fpath="/root/capdat.test.txt";
    char cmd[256]={0};
    sprintf(cmd,"echo kkk > %s",fpath);
    printf("syscmd: %s",cmd);
    system(cmd);

    fd = open(fpath,O_RDWR|O_CREAT|O_APPEND);
    write(fd,cmd,strlen(cmd));
    close(fd);
}

int capsuid()
{
    if(setuid(74) == -1){
        perror("\033[0;31mwhen setuid(74)");
        printf("\033[0m");
    }else{
        printf("after setuid(74)");
        printf("uid:%d,euid:%d\n",getuid(),geteuid());
        printf("try read file 2\n");
        fileread();
    }
}

int main(int argc,char** argv)
{
    printf("uid:%d,euid:%d\n",getuid(),geteuid());
    printf("try read file 1\n");
    fileread();

    capdac();

    return 0;
}

