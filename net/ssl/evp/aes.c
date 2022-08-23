#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <openssl/evp.h>
#include <sys/random.h>

int main(int argc,char** argv)
{
    char *in;
    char *out;
    char iv[16];
    char key[16];
    int ilen,olen,tmplen;
    int ret;
    EVP_CIPHER_CTX *ctx;

    long int count;
    struct timespec ts,te;
    clockid_t clkid = CLOCK_REALTIME;

    count = 0xffff;
    if (argc >1 ){
        count = strtol(argv[1],NULL,0);
    }

    printf("count:%ld/%#lx\n",count,count);

    ilen = 16;
    in = malloc(ilen);

    olen = ilen+16;
    out = malloc(olen);

    ret = getrandom(in, ilen,0);
    ret = getrandom(key, 16,0);
    ret = getrandom(iv, 16,0);

    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

    ret = clock_gettime(clkid,&ts);
    printf("start time sec:nsec,%ld:%ld\n",ts.tv_sec,ts.tv_nsec);

    while (count--) {
        if (!EVP_EncryptUpdate(ctx, in, &olen, in, ilen)) {
            EVP_CIPHER_CTX_free(ctx);
            printf("evp update error\n");
            return 0;
        }
    }

    ret = clock_gettime(clkid,&te);
    printf("end time sec:nsec,%ld:%ld\n",te.tv_sec,te.tv_nsec);

    long ndiff = 1*1000000000 + te.tv_nsec - ts.tv_nsec;
    time_t sdiff = te.tv_sec - 1 - ts.tv_sec + ndiff/1000000000;
    printf("escape %d sec, %ld nsec\n",sdiff,ndiff%1000000000);
    /*
     * Buffer passed to EVP_EncryptFinal() must be after data just
     * encrypted to avoid overwriting it.
     */
    if (!EVP_EncryptFinal_ex(ctx, out + olen, &tmplen)) {
        EVP_CIPHER_CTX_free(ctx);
        printf("evp final error\n");
        return 0;
    }
    EVP_CIPHER_CTX_free(ctx);

}
