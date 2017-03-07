#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>

int init_module(void)
{
	int retries = 0x100;
	int input = 0;
	printk("This is a kernel module\n");
	while (--retries != 0){
		int oldinput=input; 
		input=inb(0x60); 
		if (oldinput!=input)
			printk("got %i",input);
	};
	return 0;
}

void cleanup_module(void)
{
	  printk(KERN_ALERT "Au revoir\n");
}

MODULE_AUTHOR("rowan pang <pangweizhen.2008@hotmail.com");
MODULE_DESCRIPTION("input keyboard test");
MODULE_LICENSE("GPL");

/*module_init(init_module); */
/*module_exit(cleanup_module); */

