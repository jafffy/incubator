#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include "peripheral.h"


/************************************************************************/
// 
//  color LED  driver region  
//
/************************************************************************/
volatile unsigned short  *cled_base;

int cled_open(struct inode *inode, struct file *pfile) 
{

	printk("[cled_open] - success.\n");
	if (request_mem_region(COLOR_LED_BASE_ADDRESS ,ADDRESS_MAP_SIZE,"CNCLED") == NULL)
		return -EBUSY;

	cled_base = ioremap(COLOR_LED_BASE_ADDRESS ,ADDRESS_MAP_SIZE);
	if ( cled_base == NULL)
	{
		release_mem_region(COLOR_LED_BASE_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	
		
	return 0;
}

ssize_t cled_read(struct file *pfile, char *buf, size_t count, loff_t *filePos)
{
	unsigned short rdata[4];
	unsigned int ret;
	rdata[0] = readw(cled_base);
	rdata[1] = readw(cled_base + 1);
	rdata[2] = readw(cled_base + 2);

	ret = copy_to_user(buf,(unsigned char *)rdata,6);	
	return 6;
}
ssize_t cled_write(struct file *pfile, const char *buf,size_t count, loff_t *filePos)
{

	unsigned short wdata[4];
	unsigned int ret;
	int index ;
	ret = copy_from_user((unsigned char*)wdata ,buf,8);	
	index = 8*wdata[0];
	writew(wdata[1],cled_base + index );
	writew(wdata[2],cled_base + index + 1 );
	writew(wdata[3],cled_base + index + 2 );
	printk("red:%d ,green:%d , blue:%d \n",wdata[1],wdata[2],wdata[3]);
	return count;
}

int cled_release(struct inode *inode , struct file *pfile)
{

	if (cled_base != NULL )
	{
		iounmap(cled_base);
		cled_base = NULL;
		release_mem_region(COLOR_LED_BASE_ADDRESS , ADDRESS_MAP_SIZE);

	}
	return 0;
}


struct file_operations cled_fops = {
	.owner		= THIS_MODULE,
	.open		= cled_open,
	.write		= cled_write,
	.read		= cled_read,
	.release	= cled_release
};

struct miscdevice cled_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME_COLOR_LED,
    .fops = &cled_fops,
};

static int __init cled_init(void)
{
    int error;

    error = misc_register(&cled_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    return 0;
}

static void __exit cled_exit(void)
{
    misc_deregister(&cled_device);
}

module_init(cled_init)
module_exit(cled_exit)

MODULE_DESCRIPTION("color led Driver");
MODULE_AUTHOR("cndi");
MODULE_LICENSE("GPL");

