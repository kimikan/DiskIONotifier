
/* written by jkan */


#include <linux/init.h>    
#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/fs.h>  
#include <asm/uaccess.h> 
#include <linux/spinlock.h> 

#include <linux/sched.h>  
#include <linux/types.h>  
#include <linux/fcntl.h>  
#include <linux/hdreg.h>  
#include <linux/genhd.h>  
#include <linux/blkdev.h>  

#ifndef __OPS_H_
#define __OPS_H_

# define spin_lock_init2(lock)  do { *(lock) = SPIN_LOCK_UNLOCKED; } while (0) 

void __fops_dev_transfer(unsigned long sector, unsigned long nsect, char *buffer, int write);

void __fops_dev_request_func(struct request_queue *q);

int __fops_dev_ioctl(struct block_device *dev, fmode_t no, unsigned cmd, unsigned long arg);
  
int __fops_dev_open (struct block_device *dev , fmode_t no);

void __fops_dev_release(struct gendisk *gd, fmode_t no);

int __fops_media_changed(struct gendisk *gd);

#endif
