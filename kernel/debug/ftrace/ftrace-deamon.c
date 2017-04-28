#include <linux/module.h>

static int ftrace_daemon_init(void)
{
    trace_printk("ftrace daemon \n");
    pr_info("function:%s\n",__FUNCTION__);
    return 0;
}

static void ftrace_daemon_exit(void)
{
    trace_printk("ftrace daemon unloading\n");
    pr_info("function:%s\n",__FUNCTION__);
    return ;
}

module_init(ftrace_daemon_init);
module_exit(ftrace_daemon_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("pangwz");
MODULE_DESCRIPTION("for ftrace test");
