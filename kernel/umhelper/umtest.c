#include <linux/module.h>
#include <linux/init.h>
#include <linux/kmod.h>

MODULE_LICENSE( "GPL" );

static int umh_test( void )
{
    int ret;
    char *argv[] = { 
	"/home/pangwz/noteGit/gnu/io/file", 
	"/home/pangwz/noteGit/gnu/io/fd1",
	 NULL 
    };

    static char *envp[] = {
	"HOME=/",
	"TERM=linux",
	"PATH=/sbin:/bin:/usr/sbin:/usr/bin", 
	NULL 
    };

    /*char *argv[] = { "/usr/bin/logger", "pangwzhelp!", NULL };*/
    /*static char *envp[] = {*/
        /*"HOME=/",*/
        /*"TERM=linux",*/
        /*"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };*/

    ret = call_usermodehelper( argv[0], argv, envp, UMH_WAIT_EXEC);
    if(!ret){
	pr_info("umh ok!!!\n");
    }else{
	pr_info("umh error!!!\n");
    }

    return ret;
}

static int __init mod_entry_func( void )
{
    return umh_test();
}


static void __exit mod_exit_func( void )
{
    return;
}

module_init( mod_entry_func );
module_exit( mod_exit_func );
