#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <libgen.h>
#include <errno.h>

#define dbgprintf(fmt,arg...)    \
    if(debug >= 1){                    \
        printf(fmt,##arg);        \
    }

#define ddbgprintf(fmt,arg...)    \
    if(debug >= 2){                    \
        printf(fmt,##arg);        \
    }

#define DEF_OUTBUF_SIZE 64
int usage(char *prog){
    printf("Usage:%s [options] textfile\n",prog);
    printf("    -h:help\n");
    printf("    -d:debug,-dd for more debug message\n");
    printf("    -s:separater char,[no need]\n");
    printf("    -o:output file name,[out.bin]\n");
    return 0;
}

int main(int argc,char** argv)
{
    char sepa = ',';
    int ret = 0;    
    char *prog = basename(argv[0]);
    char debug = 0;
    char *outfilepath = "out.bin";

    FILE *outfp = NULL;
    char *infilepath = NULL;    
    char *outbuf = NULL;
    int outbufsize = 0;
    int outindex = 0;
    FILE *infp = NULL;
    char *linebuf = NULL;
    size_t inbufsize = 0;
    ssize_t nm_read = 0;
    int line_nm = 0;

    while((ret = getopt(argc,argv,"dhs:o:")) != -1){
        switch(ret){
            case 'h':
                usage(prog);
                goto OUT;
                break;
            case 's':
                sepa = *optarg;
                break;
            case 'd':
                debug += 1;
                break;    
            case 'o':
                outfilepath = optarg;    
                break;
            default:
                usage(prog);
                exit(EXIT_FAILURE);
                break;
        }
    }
    if(optind >= argc){
        usage(prog);
        ret = EXIT_FAILURE;
        goto OUT;
    }

    infilepath = argv[optind];    
    dbgprintf("debug is:%d\n",debug);
    dbgprintf("separator is:'%c',infile:%s,outfile:%s\n",sepa,infilepath,outfilepath);
    
    outbufsize = DEF_OUTBUF_SIZE;
    outbuf = malloc(outbufsize);
    memset(outbuf,0,outbufsize);
     
    infp = fopen(infilepath,"r");
    if(infp == NULL){
        char str[40];
        snprintf(str,40,"when open file %s",infilepath);
        perror(str);
        ret = errno;
        goto OUT;
    }

    while((nm_read = getline(&linebuf,&inbufsize,infp)) != -1){
        ddbgprintf("line %d,count %d:    %s",line_nm,nm_read,linebuf);
        line_nm++;
        linebuf[nm_read]='\0';
        linebuf[nm_read+1]='\0';        //deal the last line of the file no '\n',just 'eof' 
        char *start = linebuf;
        char *endptr = NULL;
        int data = 0;
    #if 0    
        while((endptr = strchr(start,sepa)) != NULL){
            *endptr++ = '\0';
            printf("%s ",start);
            start = endptr;
        }
    #else
        while(1){
            errno = 0;
            data = strtol(start,&endptr,16);
            if((errno != 0)||(endptr  == start)){
                break;
            }
            *endptr = '\0';
            ddbgprintf("str:%s,data:%x ",start,data);
            outbuf[outindex++] = data;
            start = ++endptr;

            if(outindex >= outbufsize){
                outbuf = realloc(outbuf,outbufsize+DEF_OUTBUF_SIZE);
                outbufsize += DEF_OUTBUF_SIZE;
                ddbgprintf("outbuf realloc\n");
            }
        }
    #endif
        ddbgprintf("\n");
        //memset(linebuf,0,inbufsize);    //getline not do clean,need manual clear the linebuf, you can also do linebuf[nm_read...nm_read+1]='\0'
    }
    
    printf("need write:%d bytes\n",outindex);

    outfp = fopen(outfilepath,"w");
    if(outfp == NULL){
        char str[40]={0};
        snprintf(str,40,"when open file %s",outfilepath);
        perror(str);
        ret = errno;
        goto OUT;
    }    

    int size = fwrite(outbuf,outindex,1,outfp);    
    printf("write:%d bytes\n",size*outindex);

OUT:
    if(outfp){
        fflush(outfp);
        fclose(outfp);
    }
    if(infp){
        fclose(infp);
    }
    if(linebuf){
        free(linebuf);
    }
    if(outbuf){
        free(outbuf);
    }
    return ret;
}
