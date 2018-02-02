#include<stdio.h>
#include<string.h>
#include<openssl/evp.h>
#include<openssl/hmac.h>

int main(int argc,char** argv)
{
    int ret;
    char *key = "keyforhmac";
    char *msg = "msg-for-hmac-sha256";
    unsigned char hmac[EVP_MAX_MD_SIZE];
    unsigned int hmacLen;
    unsigned char *hret;

    unsigned kd[128];
    int kdTmpLen=0;

    EVP_MD const *hash = EVP_sha256();

    printf("max hash size in byt:%d\n",EVP_MAX_MD_SIZE);
    printf("as \"echo -n '%s' | sha256hmac -K '%s' -\"\n",msg,key);

    hret = HMAC(hash,key,strlen(key),(unsigned char*)msg,strlen(msg),hmac,&hmacLen);
    hret = hret;
	/*hret also point to the hmac value,a static arrary*/

    printf("hmacLen:%d,val in hex:\n",hmacLen);
    for(int i=0; i<hmacLen; i++){
	/*printf("%02x,%02x ",hmac[i],hret[i]);*/
	printf("%02x ",hmac[i]);
	if (i && (i+1)%16 == 0){
	    putchar('\n');
	}
    }

    memcpy(kd+kdTmpLen,hmac,hmacLen);
    kdTmpLen += hmacLen;

    ret = 0;
    return ret;
}
