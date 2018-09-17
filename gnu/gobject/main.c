#include <stdio.h>
#include "sayHello.h"

int main(int argc,char **argv)
{
    sayHello *hello = sayHello_new();
    sayHello *hello2 = sayHello_new();
    printf("in func:%s,hello->msg:%s\n",__func__,hello->msg);

    say_hello_set_msg(hello,"hello");
    say_hello_set_msg(hello2,"hello2");
    printf("in func:%s,hello->msg:%s\n",__func__,hello->msg);
    printf("in func:%s,hello->msg:%s\n",__func__,hello2->msg);

    sayHelloClass *hc = SAY_HELLO_GET_CLASS(hello);
    hc->sayHello(hello);
    hc->sayHello(hello2);

    return 0;
}
