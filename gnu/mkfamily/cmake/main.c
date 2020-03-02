#include <stdio.h>
#include <hello.h>  //samedir

#include <foo.h>    //under my-build

int main()
{
    sayHello();
    printf("how are you?\n");
#ifdef FOO_ENABLE
    printf("foo str:%s\n",FOO_STRING);
#endif
    return 0;
}
