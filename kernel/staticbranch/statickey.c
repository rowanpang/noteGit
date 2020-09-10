#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/vermagic.h>

#define core_param(name,var,type,perm)

int count=0;
char *who="who-default";
char *whoCore="whoCore-default";

module_param(count,int,0644);
module_param(who,charp,0644);
core_param(whoCore,whoCore,charp,0644);

//DEFINE_STATIC_KEY_FALSE(sayhello);
DEFINE_STATIC_KEY_TRUE(sayhello);

static int noinline say_hello(void)
{
    printk(KERN_DEBUG"hello from file:%s\n",__FILE__);
    return 0;
}

static int noinline myelsemsg(int elsea)
{
    printk("in else: %#x\n",elsea);
    return 0;
}

static int noinline statickeybranchunlikely(int test)
{
    int elsea = 0;

    if (test) {
        elsea = 0x1111;
    }

    if (static_branch_unlikely(&sayhello)) {
        say_hello();
    } else{
        myelsemsg(elsea);
        elsea = 0x11;
    }

    elsea = 0x2222;
    printk("end:%d\n",elsea);

    return 0;
}

static int noinline statickeybranchlikely(int test)
{
    int elsea = 0;

    if ( test ) {
        elsea = 0x9999;
    }

    if (static_branch_likely(&sayhello)) {
        say_hello();
    } else {
        myelsemsg(elsea);
        elsea = 0x99;
    }

    //static_branch_enable(&sayhello);
    if ( test ) {
        static_branch_disable(&sayhello);
    }

    return 0;
}

static int noinline myentry(void)
{
    statickeybranchlikely(count);
    statickeybranchunlikely(count);

    return 0;
}

static int __init  example_init(void)
{
    printk("in func :%s\n",__func__);

    myentry();

    pr_info("kernel vermagic:%s\n",VERMAGIC_STRING);
    printk("module_param int count:%d\n",count);
    printk("module_param char *who:%s\n",who);

    return 0;
}

static void __exit example_exit(void)
{
    printk("exit in func :%s\n",__func__);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kmod example");
MODULE_AUTHOR("rowan pang");

module_init(example_init);
module_exit(example_exit);
