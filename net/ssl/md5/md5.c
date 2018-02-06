#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

int main(int argc, char *argv[])
{
    int ret;
    int i;
    unsigned char md[MD5_DIGEST_LENGTH];
    unsigned char mdKnow[MD5_DIGEST_LENGTH];
    unsigned int mdlen = MD5_DIGEST_LENGTH;
    /*len:50*/
    unsigned char msg[64] = {
	'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
	'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
	'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
	'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
    };

    unsigned char *append="append";

    MD5_CTX digestInstance;
    MD5_CTX *digest = &digestInstance;

    printf("----------3key+50msg--------------\n");
    ret = MD5_Init(digest);
    ret = MD5_Update(digest,"key",3);
    ret = MD5_Update(digest,msg,50);
    ret = MD5_Final(mdKnow,digest);
    printf("digest len:%d,val:\n",mdlen);
    for(i=0;i<mdlen;i++){
	printf("%02x",mdKnow[i]);
	if(i && !((i+1) & 0x03)){
	    printf(" ");
	    /*94 34 13 c8 b6 5c c1 20  e 34 78 cd fa 82 b7 d5*/
	}
	if(i && !((i+1) & 0x0f)){
	    printf("\n");
	}
    }

    printf("\n----------3key+50msg+3padding+8len+'append'--------------\n");
    ret = MD5_Init(digest);
    ret = MD5_Update(digest,"key",3);
    ret = MD5_Update(digest,msg,50);
    /*padding:0x80 00 00*/
    ret = MD5_Update(digest,"\x80\x00\x00",3);
    /*padding length in bits: 53*8,0x35,lsb*/
    ret = MD5_Update(digest,"\xa8\x01\x00\x00\x00\x00\x00\x00",8);
    ret = MD5_Update(digest,append,strlen(append));

    ret = MD5_Final(md,digest);
    printf("digest len:%d,val:\n",mdlen);
    for(i=0;i<mdlen;i++){
	printf("%02x",md[i]);
	if(i && !((i+1) & 0x03)){
	    printf(" ");
	    /*94 34 13 c8 b6 5c c1 20  e 34 78 cd fa 82 b7 d5*/
	}
	if(i && !((i+1) & 0x0f)){
	    printf("\n");
	}
    }

    printf("\n----attack---64msg+'append'--------------\n");
    ret = MD5_Init(digest);
    ret = MD5_Update(digest,msg,64);

    digest->A = htole32(((int*)mdKnow)[0]);
    digest->B = htole32(((int*)mdKnow)[1]);
    digest->C = htole32(((int*)mdKnow)[2]);
    digest->D = htole32(((int*)mdKnow)[3]);
    ret = MD5_Update(digest,append,strlen(append));

    ret = MD5_Final(md,digest);
    printf("digest len:%d,val:\n",mdlen);
    for(i=0;i<mdlen;i++){
	printf("%02x",md[i]);
	if(i && !((i+1) & 0x03)){
	    printf(" ");
	    /*94 34 13 c8 b6 5c c1 20  e 34 78 cd fa 82 b7 d5*/
	}
	if(i && !((i+1) & 0x0f)){
	    printf("\n");
	}
    }

    ret = 0;
    return ret;
}
