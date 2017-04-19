
#include "ops.h"
#include "jprobe.h"
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/sched.h>

#define NETLINK_REALNET     26  
struct sock *__sock = NULL;  
struct sockaddr_nl src_addr, dest_addr;  
struct iovec iov; 
int __pid = 0;

int nl_send_msg(const u8 *data, int data_len) {
    struct nlmsghdr *rep;
    u8 				*res;
    struct sk_buff 	*skb;
	
    if(__pid <= 0 || __sock == NULL) {
       printk("Invalid parameter, g_pid = %d, g_nl_sk = %p\n",
			__pid, __sock);
       return -1;
    }
    skb = nlmsg_new(data_len, GFP_KERNEL);
    if(!skb) {
       printk("nlmsg_new failed!!!\n");
       return -1;
    }
    rep = __nlmsg_put(skb, __pid, 0, NLMSG_NOOP, data_len, 0);
    res = nlmsg_data(rep);
	//NETLINK_CB(skb).pid = 0; 
    memcpy(res, data, data_len);
    netlink_unicast(__sock, skb, __pid, MSG_DONTWAIT);  
 	
    return 0;
}  

void on_data_received(struct sk_buff *__skb) {  
	struct sk_buff *skb;  
	struct nlmsghdr *nlh;  
	char str[100];
	skb = skb_get (__skb);
	if(skb->len >= NLMSG_SPACE(0)) {
		nlh = nlmsg_hdr(skb);
		memcpy(str, NLMSG_DATA(nlh), sizeof(str));
		if(str[0] == 'X') {
			__pid = nlh->nlmsg_pid;		
		}
		printk("Message received(%d): %s\n", nlh->nlmsg_pid, str);
		kfree_skb(skb);
	}  
}  

void net_create(void) {
	struct netlink_kernel_cfg cfg = {  
        .groups	= 1,
		.input= on_data_received,  
    };  
    __sock = netlink_kernel_create(&init_net, NETLINK_REALNET, &cfg);  
    if (!__sock) {  
        printk("create NETLINK_GENERIC fail!\n");  
    }  
}

void net_release(void) {
	if(__sock) {
		netlink_kernel_release(__sock);	
		__sock = NULL;
	}
}

int __fops_media_changed(struct gendisk *gd) { return 0; }


/* data transfer */  
void __fops_dev_transfer(unsigned long sector, unsigned long nsect, char *buffer, int write) {
}  
  
/* request handler */  
void __fops_dev_request_func(struct request_queue *q) {
	//struct request *req;
}  
  
#define START 100
#define STOP 110

static hooker_t* __hook = NULL;

void release_hooker(void) {

	if(__hook) {
		hooker_release(&__hook);
		printk("Hooker release! \n");
		__hook = NULL;
	}
}

/*strcut block_device_operations*/  
int __fops_dev_ioctl(struct block_device *dev, fmode_t no, unsigned cmd, unsigned long arg) {  
	
		printk("ioctl cmd: %d\n", cmd);
		switch(cmd) {
		case START:
			__hook = hooker_create();
			if(__hook) {
				return 0;
			}
			break;
			
		case STOP:
			if(__hook) {
				hooker_release(&__hook);
				printk("Hooker release! \n");
				return 0;
			}
			break;
			
		default:
			break;
		}
		
       return -ENOTTY;  
}  
  
int __fops_dev_open (struct block_device *dev , fmode_t no) {  
    printk("blk mount succeed\n");  
    return 0;  
}  

void __fops_dev_release(struct gendisk *gn, fmode_t no) {  
    printk("blk umount succeed\n");  
} 
