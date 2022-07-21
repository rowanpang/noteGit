#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <endian.h>
#include <bits/endian.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <zlib.h>

int main(int argc,char** argv)
{
    int ret = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    Bytef *inbuf = "rowan pangweizhenbj beijing";
    if(argc > 1){
        inbuf = argv[1];
    }
    uLong inlen = strlen(inbuf);
    Bytef *outbuf;
    uLong outlen;
    Byte tmp;

    outlen = compressBound(inlen);
    outbuf = malloc(outlen);

    ret = compress(outbuf,&outlen,inbuf,inlen);

    printf("compress finish,ret :%x,inlen:%ld,outlen:%ld\n",ret,inlen,outlen);

    printf("\t");
    for(i=0,j=0;i<outlen;){
        tmp = outbuf[i];
        i++;                                                                    //提前i++,而不是在for(;;)里,便于后面取整判断.
        if (argc>2){                                                           //默认不输出binary,
            printf("%02x",tmp);
            printf("(b");
            for(k=0;k<8;k++){
                if(tmp&0x80){
                    printf("%c",'1');
                } else {
                    printf("%c",'0');
                }
                tmp<<=1;
            }
            printf(") ");
        } else {
            printf("%02x ",tmp);
        }

        if(i%16 == 0){                                                          //完整行输出,输出可读字符.
            printf("\t");
            for(;j<i;j++){
                if( '0' <= outbuf[j] && outbuf[j] <= 'Z'){
                    printf("%c",outbuf[j]);
                } else {
                    printf("%c",'.');
                }
            }
            if(i!=outlen){                                                      //如正好是整数行,最后一行不再输出"\n"
                printf("\n\t");
            }
        }
    }

    if(i%16){                                                                   //非整数行输出,最后一行可读字符输出.
        for(k=0;k<(((16-(i%16))*3)+7)/8 + 1;k++){                               //display app 如(tmux) 处理't'时回自动对齐到一个制表位.
            printf("\t");                                                       //先补齐"\t"
        }
    }

    for(;j<i;j++){
        if( '0' <= outbuf[j] && outbuf[j] <= 'Z'){
            printf("%c",outbuf[j]);
        } else {
            printf("%c",'.');
        }
    }
    printf("\n");
}


