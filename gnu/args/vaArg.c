#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdarg.h>

void foo2(char *fmt, ...)
{
    return;
}

void foo(char *fmt, ...)
{
    va_list ap;
    int d;
    long l;
    char c, *s;
    if(!fmt){
	return;
    }
    va_start(ap, fmt);
    while (*fmt)
	switch (*fmt++) {
	case 's':              /* string */
	    s = va_arg(ap, char *);
	    printf("string %s\n", s);
	    break;
	case 'd':              /* int */
	    d = va_arg(ap, int);
	    printf("int %d\n", d);
	    break;
	case 'l':              /* int */
	    l = va_arg(ap, long);
	    printf("long %lx\n", l);
	    break;
	case 'c':              /* char */
	   /* need a cast here since va_arg only
	      takes fully promoted types */
	    c = (char) va_arg(ap, int);
	    printf("char %c\n", c);
	    break;
    }
    va_end(ap);
}


int main(int arg,char **argv)
{
    int ret;

    foo(argv[1]);

    ret = 0;
OUT:
    return ret;
}
