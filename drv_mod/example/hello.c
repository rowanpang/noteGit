#include <linux/module.h>

int say_hello(void)
{
	printk(KERN_DEBUG"hello from file:%s\n",__FILE__);
	return 0;
}
