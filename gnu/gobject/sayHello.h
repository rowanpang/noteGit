#ifndef _say_hello_H_
#define _say_hello_H_
#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _sayHello	sayHello;
typedef struct _sayHelloClass   sayHelloClass;

struct _sayHello{
    GObject parent_instance;
    char *msg;
};

struct _sayHelloClass{
    GObjectClass parent;
    void (*sayHello)(sayHello *obj);
};

#define TYPE_SAY_HELLO		(say_hello_get_type())
#define SAY_HELLO(o)		(G_TYPE_CHECK_INSTANCE_CAST((o), TYPE_SAY_HELLO, sayHello))
#define SAY_HELLO_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o),TYPE_SAY_HELLO,sayHelloClass))

GType say_hello_get_type(void);

sayHello* sayHello_new(void);
void say_hello_set_msg(sayHello* klass,char *msg);

G_END_DECLS
#endif
