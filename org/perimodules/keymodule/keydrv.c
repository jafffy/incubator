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
//  Key pad  driver region 
//
/************************************************************************/
volatile unsigned short  *key_base;


int key_open(struct inode *inode, struct file *pfile) 
{

//	printk("[key_open] - success.\n");
	if (request_mem_region(KEYPD_BASE_ADDRESS ,ADDRESS_MAP_SIZE,"CNLED") == NULL)
		return -EBUSY;

	key_base = ioremap(KEYPD_BASE_ADDRESS ,ADDRESS_MAP_SIZE);
	if ( key_base == NULL)
	{
		release_mem_region(KEYPD_BASE_ADDRESS, ADDRESS_MAP_SIZE);
		return -ENOMEM;
	}
	
		
	return 0;
}

ssize_t key_read(struct file *pfile, char *buf, size_t count, loff_t *filePos)
{
	unsigned short rdata;
	unsigned int ret;
	int btnNo = 0;
	int i,j;
	// read key
	for(i = 0; i < 4 ;i++ )
	{
		writew(0x10 << i,key_base );
		rdata =readw(key_base) & 0xF;
//		printk("read :0x%X\n",rdata);
		if ( rdata > 0 ) 
		{
			for(j = 0; j < 4 ; j++ ) 
			{
				 if ( rdata & (0x01<< j))
				 {
					btnNo = i + 1 + j*4;
					break;
				 }
			}
		}

		if ( btnNo )
		{
			break;
		}

	}

	ret = copy_to_user(buf,&btnNo,4);	
	return 4;
}
ssize_t key_write(struct file *pfile, const char *buf,size_t count, loff_t *filePos)
{

	unsigned short wdata;
	unsigned int ret;
	ret = copy_from_user(&wdata ,buf,2);	
	writew(wdata,key_base);
	return count;
}


int key_release(struct inode *inode , struct file *pfile)
{

	if (key_base != NULL )
	{
		iounmap(key_base);
		key_base = NULL;
		release_mem_region(KEYPD_BASE_ADDRESS , ADDRESS_MAP_SIZE);

	}
	return 0;
}


struct file_operations key_fops = {
	.owner		= THIS_MODULE,
	.open		= key_open,
	.write		= key_write,
	.read		= key_read,
	.release	= key_release
};

struct miscdevice key_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEV_NAME_KEYPD,
    .fops = &key_fops,
};

static int __init key_init(void)
{
    int error;

    error = misc_register(&key_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    return 0;
}

static void __exit key_exit(void)
{
    misc_deregister(&key_device);
}

module_init(key_init)
module_exit(key_exit)

MODULE_DESCRIPTION("key Driver");
MODULE_AUTHOR("cndi");
MODULE_LICENSE("GPL");


