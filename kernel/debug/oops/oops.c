#include <linux/module.h>

static int oops_test_init(void)
{
	int *p = 0;
	*p = 1;
	return 0;
}

static void oops_test_exit(void)
{
	return ;
}

module_init(oops_test_init);
module_exit(oops_test_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("pangwz");
MODULE_DESCRIPTION("for oops test");

