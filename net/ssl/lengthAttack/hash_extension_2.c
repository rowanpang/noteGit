#include <stdio.h>
#include <endian.h>
#include <byteswap.h>
#include <arpa/inet.h>
#include <openssl/md5.h>

int main(int argc, const char *argv[])
{
    int i;
    unsigned char buffer[MD5_DIGEST_LENGTH];
    MD5_CTX c;

    MD5_Init(&c);
    MD5_Update(&c, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", 64);

    c.A = htonl(0x6036708e); /* <-- This is the hash we already had */
    c.B = htonl(0xba0d11f6);
    c.C = htonl(0xef52ad44);
    c.D = htonl(0xe8b74d5b);

    c.A = bswap_32(0x6036708e); /* <-- This is the hash we already had */
    c.B = bswap_32(0xba0d11f6);
    c.C = bswap_32(0xef52ad44);
    c.D = bswap_32(0xe8b74d5b);

    MD5_Update(&c, "append", 6); /* This is the appended data. */
    MD5_Final(buffer, &c);
    for (i = 0; i < 16; i++) {
	printf("%02x", buffer[i]);
    }
    printf(" ");
    return 0;
}
