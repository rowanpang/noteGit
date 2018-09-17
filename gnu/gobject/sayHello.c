#include <glib.h>
#include "sayHello.h"

G_DEFINE_TYPE(sayHello,say_hello,G_TYPE_OBJECT);

void mySayHello(sayHello *obj)
{
    g_print("in func:%s, ", __func__);
    g_print("%s\n",obj->msg);
}

static void say_hello_class_init(sayHelloClass* klass)
{
    g_print("in func:%s\n", __func__);
    klass->sayHello = mySayHello;
}

static void say_hello_init(sayHello* klass)
{
    g_print("in func:%s\n", __func__);
    klass->msg = "kkkkk";
}

void say_hello_set_msg(sayHello* klass,char *msg)
{
    g_print("in func:%s\n", __func__);
    klass->msg = msg;
}


sayHello* sayHello_new(void)
{

    return g_object_new(TYPE_SAY_HELLO,NULL);
}
