#include <stdio.h>
#include "hello.h"
#include "dyn.h"
#include "msg.h"

int main()
{
    printf("how are you?\n");
    dyn();

#ifndef NO_SAYHELLO
    sayHello();
#endif
    msg_direct();
    return 0;
}
