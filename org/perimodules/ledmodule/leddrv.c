#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include "peripheral.h"

volatile unsigned short  *led_base;
int led_open(struct inode *inode, struct file *pfile) 
{

	printk("[led_open] - success.\n");
	if (request_mem_region(LED_BASE_ADDRESS ,ADDRESS_MAP_SIZE,"CNLED") == NULL)
		return -EBUSY;

	led_base = ioremap(LED_BASE_ADDRESS ,ADDRESS_MAP_SIZE);
	if ( led_base == NULL)
	{
		release_mem_region(LED_BASE_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	
		
	return 0;
}

ssize_t led_read(struct file *pfile, char *buf, size_t count, loff_t *filePos)
{
	unsigned short wdata;
	unsigned int ret;
	wdata = readw(led_base);
	ret = copy_to_user(buf,&wdata,2);	
	return 2;
}
ssize_t led_write(struct file *pfile, const char *buf,size_t count, loff_t *filePos)
{

	unsigned short wdata;
	unsigned int ret;
	ret = copy_from_user(&wdata ,buf,2);	
	writew(wdata,led_base);
	return count;
}


int led_release(struct inode *inode , struct file *pfile)
{

	if (led_base != NULL )
	{
		iounmap(led_base);
		led_base = NULL;
		release_mem_region(LED_BASE_ADDRESS , ADDRESS_MAP_SIZE);

	}
	return 0;
}


struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.open		= led_open,
	.write		= led_write,
	.read		= led_read,
	.release	= led_release
};

struct miscdevice led_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME_LED,
    .fops = &led_fops,
};

static int __init led_init(void)
{
    int error;

    error = misc_register(&led_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    return 0;
}

static void __exit led_exit(void)
{
    misc_deregister(&led_device);
}

module_init(led_init)
module_exit(led_exit)

MODULE_DESCRIPTION("led Driver");
MODULE_AUTHOR("cndi");
MODULE_LICENSE("GPL");

