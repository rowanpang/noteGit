#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/printk.h> 

int doIt = 0;
module_param(doIt,int,0600);

static bool intercept_filter(struct input_handle *handle, unsigned int type, unsigned int code, int value)
{
    static char toFilter=3;
    char ret = 0;
#define evenTypeStrNum 5
    char *typeStr[evenTypeStrNum]={
	"sync",
	"key",
	"rel",
	"abs",
	"msc",
    };
    if (toFilter == 0){
	ret = 1;
    }else{
	ret = 0;
	toFilter--;
    }
    if (type < evenTypeStrNum){
	pr_info("input--:event type:%s,code:%d,value:%d\n",typeStr[type],code,value);
    }else{
	pr_info("input--:event type %d,code %d,value:%d. --ret:%d,toFilter:%d\n",type,code,value,ret,toFilter);
    }

    if(!doIt){
	ret = 0;
    }

    return ret;
}

static int intercept_connect(struct input_handler *handler, struct input_dev *dev, const struct input_device_id *id)
{  
    struct input_handle *handle;                                                                                                                   
    int error;

    handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
    if (!handle)
	    return -ENOMEM;

    handle->dev = dev;
    handle->handler = handler;
    handle->name = "intercept-handle";

    error = input_register_handle(handle);
    if (error)
	goto err_free_handle;

    error = input_open_device(handle);
    if (error)
	goto err_unregister_handle;
	    
    return 0;

err_unregister_handle:
    input_unregister_handle(handle);
err_free_handle:
    kfree(handle);
    return error;
}

static void intercept_disconnect(struct input_handle *handle)
{
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
}

static const struct input_device_id intercept_ids[] = {
    {
	.flags = INPUT_DEVICE_ID_MATCH_EVBIT|INPUT_DEVICE_ID_MATCH_KEYBIT,
	.evbit = { BIT_MASK(EV_KEY) },
	.keybit = { BIT_MASK(KEY_LEFTALT) },
    },
    { },
};

static struct input_handler intercept_handler = {
        .filter         = intercept_filter,
        .connect        = intercept_connect,
        .disconnect     = intercept_disconnect,
        .name           = "intercept-handler",
        .id_table       = intercept_ids,
};

static bool intercept_handler_registed = false;

static int __init  example_init(void)
{
    int error;
    pr_info("in func :%s\n",__func__);
    error = input_register_handler(&intercept_handler);
    if(error){
	pr_err("intercept handler register error %d\n",error);
    }else{
	intercept_handler_registed = true;
    }

    
    return 0;
}

static void __exit example_exit(void)
{
    pr_info("exit in func :%s\n",__func__);
    if(intercept_handler_registed){
	input_unregister_handler(&intercept_handler);
	intercept_handler_registed = false;
    }
}

MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("kmod example"); 
MODULE_AUTHOR("rowan pang"); 

module_init(example_init); 
module_exit(example_exit);
