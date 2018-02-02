#include<stdio.h>
#include<string.h>
#include<openssl/evp.h>
#include<openssl/hmac.h>

int prf(unsigned char* key,unsigned char *seed,char seedlen,
	unsigned char *out,char outlen)
{
    int ret = 0;

    int outTmpLen = 0;
    unsigned char *outTmp;

    unsigned char seedTmp[EVP_MAX_MD_SIZE];
    char seedTmpLen;
    unsigned int hlen;
    unsigned char *hret;

    outTmp = malloc(outlen+EVP_MAX_MD_SIZE);
    memset(outTmp,0,outlen);

    seedTmpLen = (seedlen < EVP_MAX_MD_SIZE) ? seedlen : EVP_MAX_MD_SIZE;
    memcpy(seedTmp,seed,seedTmpLen);

    while(1){
	hret = HMAC(EVP_sha256(),key,strlen((char*)key),seedTmp,seedTmpLen,NULL,&hlen);
	memcpy(outTmp+outTmpLen,hret,hlen);
	outTmpLen += hlen;
	if( outTmpLen >= outlen){
	    break;
	}

	memcpy(seedTmp,hret,hlen);
    }

    printf("len need:%d,gen:%d\n",outlen,outTmpLen);

    memcpy(out,outTmp,outlen);
    free(outTmp);

    return ret;
}

int main(int argc,char** argv)
{
    int ret;
    char *key = "keyforhmac";
    char *msg = "msg-for-hmac-sha256";

    int kdLen = 70;
    unsigned char *kd;

    kd = malloc(kdLen);
    printf("max hash size in byt:%d\n",EVP_MAX_MD_SIZE);

    ret = prf((unsigned char*)key,(unsigned char*)msg,strlen(msg),kd,kdLen);

    printf("key derivatioin:\n");
    for(int i=0; i<kdLen; i++){
	printf("%02x ",kd[i]);
	if (i && ((i+1)&0xf) == 0){
	    printf("\n");
	}
    }

    free(kd);
    ret = 0;
    return ret;
}
