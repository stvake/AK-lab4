#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Pyrohov Pavlo <pyrohovpavlo05@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of times to print Hello, world!");

struct hello_entry
{
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	int i;

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10))
	{
		printk(KERN_WARNING "Warning: Invalid count (%u), is 0 or between 5 and 10\n", hello_count);
	}
	else if (hello_count > 10)
	{
		printk(KERN_ERR "Error: Count exceeds limit 10, exiting with -EINVAL\n");
		return -EINVAL;
	}

	for (i = 0; i < hello_count; i++)
	{
		struct hello_entry* entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry)
		{
			printk(KERN_ERR "Error: Memory allocation failed\n");
			return -ENOMEM;
		}
		entry->time = ktime_get();
		list_add_tail(&entry->list, &hello_list);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}