#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include "hello.h"

#define core_param(name,var,type,perm) 

int count=0;
char *who="who-default";
char *whoCore="whoCore-default";

module_param(count,int,0644);
module_param(who,charp,0644);
core_param(whoCore,whoCore,charp,0644);

static int __init  example_init(void)
{
	printk("in func :%s\n",__func__);
	say_hello();	
	
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
