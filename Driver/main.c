
/*
 * Written by kimi
 * 2016/11/23
 * Purpose: notify the apppication the latest information disk write .
 * Usage, application create netlink then send a message to driver to tell "pid" 
               for recv notification
 * ioctr(100) to start. 
 * the message information is something like this.
 * Message format: ****[uint64 sector_offset,uint32 sector_count,char[SDANAME]]****
 * generally SDANAME should be less than 5, 
 * So the recv buffer larger than 20 should be enough

 * Deployment: "sudo insmod diskless.ko"
 */

#include <linux/module.h>  
#include <linux/kernel.h> 
#include<linux/init.h>
#include <linux/list.h>
#include <linux/bio.h>
#include <linux/fs.h> 
#include <linux/device.h> 
#include <linux/vmalloc.h>
#include <linux/mutex.h>

#include "ops.h"

static const struct block_device_operations __fops = {
		
	.owner            = THIS_MODULE,
	.open             = __fops_dev_open,
	.release		  = __fops_dev_release,
	.ioctl 			  = __fops_dev_ioctl,
	.media_changed    = __fops_media_changed,
};

/* device struct*/
struct device_t {
	int 			_major;  
	spinlock_t 		_spin_lock;  
	struct gendisk*	_disk;
} ;

#define DEVICE_NAME "diskless"
#define DEV_MAJOR 2533

struct device_t __device;

void init_device(struct device_t* pdev) {
	if(pdev) {
		pdev->_disk = alloc_disk(1 /* partition count */);
		spin_lock_init(&(pdev->_spin_lock));
		pdev->_disk->major = DEV_MAJOR;  
		pdev->_disk->first_minor = 10; 
		pdev->_disk->fops = &__fops;  
		/* init request queue */
		pdev->_disk->queue = blk_init_queue(__fops_dev_request_func, &(pdev->_spin_lock));  
	
		snprintf(pdev->_disk->disk_name, 32, "diskless.%c", 'a');    
		//blk_queue_hardsect_size((struct request_queue*)pdev->_disk->queue, 512); /* sector size */
		set_capacity(pdev->_disk, 20);  /* 10 k*/ 
		add_disk(pdev->_disk);
	}
}


extern void net_create(void);
extern void net_release(void);

static int __init __module_init(void) {

	/* start */
	int ret = 0;
	do {
		if(register_blkdev(DEV_MAJOR, DEVICE_NAME)){ 
			ret = -EBUSY;
			break;
		}  
		init_device(&__device);
		net_create();
	} while(0);	

    printk("gendisk init success!\n");  
    return ret;  
} //end module init

extern void release_hooker(void);

/* module exit */
static void __exit __module_exit(void) {	
	/* firstly release hooker, or kernel will crash */
	release_hooker();
	if(__device._disk) {
		blk_cleanup_queue(__device._disk->queue);
		del_gendisk(__device._disk);
	}
	net_release();
    unregister_blkdev(DEV_MAJOR, DEVICE_NAME);  
    printk("block module exit succeed!\n"); 
}


module_init(__module_init);
module_exit(__module_exit);

MODULE_DESCRIPTION("disk sector inform driver");
MODULE_AUTHOR("jkan");
MODULE_LICENSE("GPL");


