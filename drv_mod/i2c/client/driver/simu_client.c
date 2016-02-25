#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>


static int simu_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("In %s\n",__FUNCTION__);
	return -1;
}

static int simu_remove(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("In %s\n",__FUNCTION__);
	return -1;
}

static const struct i2c_device_id simu_ids[]={
	{"pangwz",0},
	{"pangwz1",1},
	{"pangwz2",2},
	{}
};
MODULE_DEVICE_TABLE(i2c, simu_ids);


static struct i2c_driver simu_client = {
	.driver = {
		.name = "simu_client",
		.owner = THIS_MODULE,
	},
	.probe = simu_probe,
	.remove = __devexit_p(simu_remove),
	.id_table = simu_ids,
};


static int simu_client_init(void)
{
	i2c_add_driver(&simu_client);
}


static void simu_client_exit(void)
{
	i2c_del_driver(&simu_client);
}


module_init(simu_client_init);
module_exit(simu_client_exit);

MODULE_DESCRIPTION("Driver for most simu cilents");
MODULE_AUTHOR("rowan pang");
MODULE_LICENSE("GPL");
