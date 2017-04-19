
#include "jprobe.h"

//extern struct menu *current;

#ifdef current
#undef current
#endif

#define current get_current() 

extern int nl_send_msg(const u8 *data, int data_len);

void __hook_func2(int rw, struct bio * bio) {
	if (NULL != bio && NULL != bio->bi_io_vec) {

		unsigned int count = bio_sectors(bio);
		if (rw & WRITE) {
			char b[BDEVNAME_SIZE];
			u8 buf[20] = {0};
			*(unsigned long long*)buf = (unsigned long long)bio->bi_iter.bi_sector;
			*(unsigned*)(buf + 8) = count;
			strcat(buf + 12, bdevname(bio->bi_bdev, b));

			nl_send_msg(buf, 13 + strlen(bdevname(bio->bi_bdev, b)));
		}
		
	}
	
	jprobe_return();
}

blk_qc_t __hook_func(struct bio * bio) {

	if (NULL != bio) {
		if (bio_has_data(bio)) {

			unsigned int count;

			if (unlikely(bio_op(bio) == REQ_OP_WRITE_SAME))
				count = bdev_logical_block_size(bio->bi_bdev) >> 9;
			else
				count = bio_sectors(bio);

			if (op_is_write(bio_op(bio))) {
				char b[BDEVNAME_SIZE];
/*
				printk(KERN_DEBUG "%s(%d): %s sector %Lu on %s (%u sectors)\n",
				current->comm, task_pid_nr(current), "WRITE",
					(unsigned long long)bio->bi_iter.bi_sector,
					bdevname(bio->bi_bdev, b), count); */
				u8 buf[20] = {0};
				*(unsigned long long*)buf = (unsigned long long)bio->bi_iter.bi_sector;
				*(unsigned*)(buf + 8) = count;
				strcat(buf + 12, bdevname(bio->bi_bdev, b));

				nl_send_msg(buf, 13 + strlen(bdevname(bio->bi_bdev, b)));
			} 
		}
	}

	jprobe_return();
	return 0;
}


static struct jprobe __jprobe = {
            .entry = (kprobe_opcode_t *) __hook_func,
			.kp = {  
	            .addr = NULL, 
				.symbol_name = "submit_bio"  
        	}  
        };


hooker_t* hooker_create(void) {
	
	kprobe_opcode_t* paddr = (kprobe_opcode_t *) kallsyms_lookup_name("submit_bio");
    hooker_t* pobj = (hooker_t*)kmalloc(sizeof( hooker_t), GFP_KERNEL);
    if(pobj) {
        spin_lock_init(&pobj->_spinlock);
        if(paddr == (kprobe_opcode_t *)0xc04e6e4 /*older version printed */) {
        	__jprobe.entry = (kprobe_opcode_t *) __hook_func2;
        }
        
        pobj->_jprobe = __jprobe;
        
        if (register_jprobe(&pobj->_jprobe) >= 0) {
			printk("Register success!\n");
            return pobj;
        }
        
        printk("Hooker register failed!\n");
        kfree(pobj);
    }
    
    printk("Hooker alloc failed!\n");
    return NULL;
}


void hooker_release(hooker_t** ppobj) {
	
    if(ppobj && *ppobj) {
        hooker_t *pMe = *ppobj;
        spin_lock(&(pMe->_spinlock));
        unregister_jprobe(&pMe->_jprobe);
		printk("Unregister success!\n");
        spin_unlock(&pMe->_spinlock);
		kfree(pMe);
		*ppobj = NULL;
    } 
}

int hooker_enable(hooker_t* pMe) {
    int ret = -1;
    if(pMe) {
        spin_lock(&pMe->_spinlock);
        ret = enable_jprobe(&pMe->_jprobe);
        spin_unlock(&pMe->_spinlock);
    }

    return ret;
}

int hooker_disable(hooker_t* pMe) {

    int ret = -1;
    if(pMe) {
        spin_lock(&pMe->_spinlock);
        ret = disable_jprobe(&pMe->_jprobe);
        spin_unlock(&pMe->_spinlock);
    }

    return ret;
}
