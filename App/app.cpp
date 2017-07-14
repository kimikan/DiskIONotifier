
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <string.h>

const int START = 100;
const int STOP = 110;
const int GETDATA = 101;

const int MAX_PAYLOAD = 128;

int netlink() {
	int state;
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL; //Netlink数据包头
    struct iovec iov;
    struct msghdr msg;
    int sock_fd, retval;
    int state_smg = 0;
    // Create a socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, 26);
    if(sock_fd == -1){
        printf("error getting socket: %s", strerror(errno));
        return -1;
    }
    // To prepare binding
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); //A：设置源端端口号
    src_addr.nl_groups = 0;
    //Bind
    retval = bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
    if(retval < 0){
        printf("bind failed: %s", strerror(errno));
        close(sock_fd);
        return -1;
    }
    // To orepare create mssage
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    if(!nlh){
        printf("malloc nlmsghdr error!\n");
        close(sock_fd);
        return -1;
	}
    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; //B：设置目的端口号
    dest_addr.nl_groups = 0;
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid(); //C：设置源端口
    nlh->nlmsg_flags = 0;
    strcpy((char*)NLMSG_DATA(nlh),"XX"); //设置消息体
    iov.iov_base = (void *)nlh;
    iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
    //Create mssage
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    //send message
    printf("state_smg\n");
    state_smg = sendmsg(sock_fd,&msg,0);
    if(state_smg == -1)
    {
        printf("get error sendmsg = %s\n",strerror(errno));
    }
    memset(nlh,0,NLMSG_SPACE(MAX_PAYLOAD));
    //receive message
    printf("waiting received!\n");
    while(1){
		char* p = NULL;
        printf("In while recvmsg\n");
        state = recvmsg(sock_fd, &msg, 0);
        if(state<0)
        {
            printf("state<1");
        }
		p = (char *) NLMSG_DATA(nlh);
        printf("Received message: ull: %Lu %d, %s\n", *(unsigned long long*) p, *(unsigned*)(p+8), p + 12);
    }
    close(sock_fd);

	return 0;
}

int main() {
    int fd = 0;
    int cmd;
    int arg = 0;
    char Buf[4096];
    
    /*打开设备文件*/
    fd = open("/dev/diskless.a", O_RDWR);
    if (fd < 0) {
        printf("Open Dev Mem0 Error! %d\n", fd);
        return -1;
    }
    while(true) {
		int n;
		std::cout<<"Please input command(100, 110): "<<std::endl;
		std::cin>>n;

		int ret;
		if ((ret = ioctl(fd, n, &arg)) < 0) {
			printf("Call cmd STart fail, ret: %d\n", ret);
			return -1;
		}

		if(n == 100) {
			netlink();
		}
	}
    
    close(fd);
    return 0;    
}
