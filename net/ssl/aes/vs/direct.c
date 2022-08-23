#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <time.h>

typedef void (*block128_f) (const unsigned char *in, unsigned char *out, const void *key);

struct aes_key_st {
    unsigned int rd_key[4 * (14+ 1)];
    int rounds;
};
typedef struct aes_key_st AES_KEY;

typedef struct {
    union {
        double align;
        AES_KEY ks;
    } ks;
    block128_f block0;
    block128_f block1;
} EVP_AES_KEY;

extern void aesni_cbc_encrypt(const unsigned char *in,
                       unsigned char *out,
                       size_t length,
                       const AES_KEY *key, unsigned char *ivec, int enc);
extern void aesni_cbc_encrypt_rowan(const unsigned char *in,
                       unsigned char *out,
                       size_t length,
                       const AES_KEY *key, unsigned char *ivec, int enc);

extern void aes_p8_cbc_encrypt (const unsigned char *in, unsigned char *out,
                          size_t len, const void *key,
                          unsigned char ivec[16], int enc);

int aes_cbc_cipher(long int count)
{
    int ret;
    char* in;
    char* out;
    char iv[16];
    int ilen;
    int keylen;

    keylen = 256;
    AES_KEY key = {
	.rounds = 10 + (keylen - 128)/32 - 1, 		//10 for 128bit, 14 for 256bits, -1 for lastround
    };
    ret = getrandom(key.rd_key, 240,0);

    ilen = 16384;
    in = malloc(ilen+16);
    out = malloc(ilen+32);

    ret = getrandom(in,ilen,0);
    ret = getrandom(iv, 16,0);

    while (count--) {
#if defined(__x86_64__)
        //aesni_cbc_encrypt(in, out, ilen, &key, iv, 1);
        aesni_cbc_encrypt_rowan(in, out, ilen, &key, iv, 1);
#elif defined(__powerpc__)
        aes_p8_cbc_encrypt(in, out, ilen, &key, iv, 1);
#endif
    }

}

extern void aesni_encrypt (const unsigned char *in, unsigned char *out, const void *key);
extern void aesni_ecb_encrypt(const unsigned char *in, unsigned char *out, size_t length, const AES_KEY *key, int enc);

extern void aes_p8_encrypt (const unsigned char *in, unsigned char *out, const AES_KEY *key);

#define NO_AESNI_ECB
int aes_ecb_cipher(long int count)
{
    int ret;
    char* in;
    char* out;
    int ilen;
    int keylen,i,bl;
    block128_f block;

    bl = 16;
    keylen = 256;
    AES_KEY key = {
	.rounds = 10 + (keylen - 128)/32 - 1, 			// -1, for lastround
    };

    ret = getrandom(key.rd_key, 240,0);

    ilen = 16384;
    in = malloc(ilen+16);
    out = malloc(ilen+32);
    ret = getrandom(in,ilen,0);

#if defined(__x86_64__)
    block = aesni_encrypt;
#elif defined(__powerpc__)
    block = aes_p8_encrypt;
#endif

    while (count--) {
    #ifdef NO_AESNI_ECB
	for (i = 0; i < ilen; i += bl) {
	    block(in + i, out + i, &key);
	}
    #else 
	aesni_ecb_encrypt(in,out,ilen,&key,1);
    #endif
    }
}

int main(int argc,char **argv)
{
    int ret;
    long int count;
    struct timespec ts,te;
    clockid_t clkid = CLOCK_REALTIME;
    /*clkid = CLOCK_MONOTONIC;*/

    ret = clock_getres(clkid,&ts);

    count = 0xffff;
    if (argc >1 ){
        count = strtol(argv[1],NULL,0);
    }

    printf("count:%ld/%#lx ,res sec:nsec,%ld:%ld\n",count,count,ts.tv_sec,ts.tv_nsec);

    ret = clock_gettime(clkid,&ts);
    printf("start time sec:nsec,%ld:%ld\n",ts.tv_sec,ts.tv_nsec);

    aes_cbc_cipher(count);
    //aes_ecb_cipher(count);

    ret = clock_gettime(clkid,&te);
    printf("end time sec:nsec,%ld:%ld\n",te.tv_sec,te.tv_nsec);

    long ndiff = 1*1000000000 + te.tv_nsec - ts.tv_nsec;
    time_t sdiff = te.tv_sec - 1 - ts.tv_sec + ndiff/1000000000;
    printf("escape %d sec, %ld nsec\n",sdiff,ndiff%1000000000);


    return 0;
}
