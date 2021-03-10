#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

void myfunc(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    return;
}

int main(int argc, char *argv[])
{
    myfunc("test 0: %x %x %x \n", 0x6cc655aa,0x55aaaa55,0xa5a55a5a);
    myfunc("test 0: %x %lx \n", 0x6cc655aa, 0x55aadeadbeffaa55);
    myfunc("test 1: %s %s \n", "one", "two");
    myfunc("test 2: %s %d %c\n", "apple", 222, 'y');
    return 0;
}
