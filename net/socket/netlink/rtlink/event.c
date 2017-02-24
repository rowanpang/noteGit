#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/types.h>
#include <errno.h>


static int netlink_init(void)
{
    struct sockaddr_nl sa;
    const int buffersize = 16 * 1024 * 1024;
    int retval;
 
    memset(&sa, 0x00, sizeof(struct sockaddr_nl));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = getpid();
    sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;
 
    int fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (fd == -1) {
        printf("error getting socket: %s", strerror(errno));
        return -1;
    }

    /* set receive buffersize */
    setsockopt(fd , SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
    retval = bind(fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_nl));
    if (retval < 0) {
        printf("bind failed: %s", strerror(errno));
        close(fd);
        fd = -1;
        return -1;
    }

    return fd;
}

int nl_msg_error(struct nlmsghdr *nh)
{
    printf("nh->nlmsg_type:%d\n",nh->nlmsg_type);
    printf("nh->nlmsg_flags:#%x\n",nh->nlmsg_flags);
    printf("nh->nlmsg_pid:%d\n",nh->nlmsg_pid);
    
    return -1;
}
 
int main(int argc, char* argv[])
{
    int fd = netlink_init();
    int len;
    char buf[4096];
    struct iovec iov = { buf, sizeof(buf) };
    struct sockaddr_nl sa;
    struct nlmsghdr *nh;
    struct msghdr msg = {
	.msg_name	= &sa,
	.msg_namelen	= sizeof(sa),
	.msg_iov	= &iov, 
	.msg_iovlen	= 1,
	.msg_control	= NULL,
	.msg_controllen = 0,
	.msg_flags	= 0
    };
    while(1){
	len = recvmsg(fd, &msg, 0);
	printf("recv msg len:%d\n",len);
	for(nh = (struct nlmsghdr*)buf;NLMSG_OK(nh,len); nh = NLMSG_NEXT(nh, len)) {
	    printf("nh->nlmsg_type:%d\n",nh->nlmsg_type);
	    printf("payload len:%d\n",NLMSG_PAYLOAD(nh,len));
	    if (nh->nlmsg_type == NLMSG_DONE)
	       break;

	    if (nh->nlmsg_type == NLMSG_ERROR) {
		if (nl_msg_error(nh)){
		    break;
		}
	    }
	}

	printf("\n");
    }

    close(fd);
    return 0;
}
