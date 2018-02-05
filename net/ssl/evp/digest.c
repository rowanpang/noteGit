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

    EVP_MD_CTX *digest;

    digest = EVP_MD_CTX_new();
    const EVP_MD *alg = EVP_get_digestbyname("md5");
    ret = EVP_DigestInit(digest,alg);

    ret = EVP_DigestUpdate(digest,"abcdefg",6);
    ret = EVP_DigestFinal(digest,md,&mdlen);

    printf("digest len:%d,val:\n",mdlen);
    for(i=0;i<mdlen;){
	printf("%2x ",md[i]);

	if(i & !((++i) & 0xf)){
	    printf("\n");
	}
    }


    ret = 0;
    return ret;
}
