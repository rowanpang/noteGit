#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/evp.h>


int main(int argc, char *argv[])
{
    int ret;
    int i;
    unsigned char md[EVP_MAX_MD_SIZE];
    unsigned int mdlen;
    /*len:50*/
    unsigned char msg[64] = {
	'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
	'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
	'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a',
	'a','a'
    };

    EVP_MD_CTX *digest;

    digest = EVP_MD_CTX_new();
    const EVP_MD *alg = EVP_get_digestbyname("md5");

    ret = EVP_DigestInit(digest,alg);
    ret = EVP_DigestUpdate(digest,"key",3);
    ret = EVP_DigestUpdate(digest,msg,50);
    ret = EVP_DigestFinal(digest,md,&mdlen);

    printf("digest len:%d,val:\n",mdlen);
    for(i=0;i<mdlen;){
	printf("%2x ",md[i]);
	if(i & !((++i) & 0xf)){
	    printf("\n");
	    /*94 34 13 c8 b6 5c c1 20  e 34 78 cd fa 82 b7 d5*/
	}
    }

    /*padding:0x80 00 00*/


    ret = EVP_MD_CTX_reset(digest);


    ret = 0;
    return ret;
}
