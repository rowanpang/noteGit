#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int high_water_alloc(void **buf, size_t *bufsize, size_t newsize)
{
#define CHUNK_SIZE      256
        /*
        ¦* Goal here is to avoid unnecessary memory allocations by
        ¦* using static buffers which only grow when necessary.
        ¦* Size is increased in fixed size chunks (CHUNK_SIZE).
        ¦*/
        if (*bufsize < newsize) {
                void *newbuf;

                newsize = (newsize + CHUNK_SIZE-1) & ~(CHUNK_SIZE-1);
                newbuf = realloc(*buf, newsize);
                if (!newbuf)
                        return 1;

                *buf = newbuf;
                *bufsize = newsize;
        }
        return 0;
}

const char *quote(const char *str, const char *quote_chars)
{
        static char *quoted_str;
        static size_t quoted_str_len;
        const unsigned char *s;
        char *q;
        size_t nonpr;

        if (!str)
                return str;

        for (nonpr = 0, s = (unsigned char *)str; *s != '\0'; s++)
                if (*s == '\\' || strchr(quote_chars, *s))
                        nonpr++;
        if (nonpr == 0)
                return str;

        if (high_water_alloc((void **)&quoted_str, &quoted_str_len,
                       (s - (unsigned char *)str) + nonpr * 3 + 1))
                return NULL;
        for (s = (unsigned char *)str, q = quoted_str; *s != '\0'; s++) {
                if (*s == '\\' || strchr(quote_chars, *s)) {
                        *q++ = '\\';
                        *q++ = '0' + ((*s >> 6)    );
                        *q++ = '0' + ((*s >> 3) & 7);
                        *q++ = '0' + ((*s     ) & 7);
                } else
                        *q++ = *s;
        }
        *q++ = '\0';

        return quoted_str;
}

int main(int argc,char **argv)
{
    char s[6]="\\101";
    puts("hello");	//"auto newline"
    puts("\101\n");
    puts(s);

    puts(quote("ssss=","=\n"));

    return 0;
}
