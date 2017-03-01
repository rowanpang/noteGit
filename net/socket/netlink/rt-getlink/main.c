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
    printf("nh->nlmsg_flags:%#x\n",nh->nlmsg_flags);
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

    struct sockaddr_nl reqSa = {
	.nl_family = AF_NETLINK,
	.nl_pid = 0,
	/*.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR*/
	//if use group,need root premission.
    };
    struct {
	struct nlmsghdr nh;
	struct ifinfomsg ifmsg;
    }req;
    memset(&req,0,sizeof(req));

    req.nh.nlmsg_len	= NLMSG_LENGTH(sizeof(struct ifinfomsg));
    req.nh.nlmsg_flags	= NLM_F_REQUEST | NLM_F_DUMP;
    req.nh.nlmsg_type	= RTM_GETLINK;
    req.nh.nlmsg_seq	= 1;

    req.ifmsg.ifi_family= AF_UNSPEC;

    struct iovec reqIov = { &req, req.nh.nlmsg_len};

    struct msghdr reqMsg = {
	.msg_name	= &reqSa,
	.msg_namelen	= sizeof(reqSa),
	.msg_iov	= &reqIov, 
	.msg_iovlen	= 1, 
    };

    struct msghdr msg = {
	.msg_name	= &sa,
	.msg_namelen	= sizeof(sa),
	.msg_iov	= &iov, 
	.msg_iovlen	= 1,
	.msg_control	= NULL,
	.msg_controllen = 0,
	.msg_flags	= 0
    };
    len = sendmsg(fd,&reqMsg,0);
    if (len == -1){
	perror("when sendmsg req");
	return len;
    }
    printf("align nlh:%d\n",NLMSG_ALIGN(sizeof(struct nlmsghdr)));
    while(1){
	len = recvmsg(fd, &msg, 0);
	printf("recv msg len:%d\n",len);
	for(nh = (struct nlmsghdr*)buf;NLMSG_OK(nh,len); nh = NLMSG_NEXT(nh, len)) {
	    printf("nh->nlmsg_type:%d\n",nh->nlmsg_type);
	    printf("payload len:%d\n",NLMSG_PAYLOAD(nh,0));
	    if (nh->nlmsg_type == NLMSG_DONE)
	       break;

	    if (nh->nlmsg_type == NLMSG_ERROR) {
		if (nl_msg_error(nh)){
		    break;
		}
	    }

	    if (nh->nlmsg_type == RTM_NEWLINK) {
		struct ifinfomsg *ifmsg = NLMSG_DATA(nh);
		printf("align ifinfomsg:%d\n",NLMSG_ALIGN(sizeof(struct ifinfomsg)));
		printf("ifinfomsg.ifi_family:%d\n",ifmsg->ifi_family);
		printf("ifinfomsg.ifi_type:%d\n",ifmsg->ifi_type);

		struct rtattr *rta = (struct rtattr*)((char *)ifmsg + NLMSG_ALIGN(sizeof(struct ifinfomsg)));
		int rtlen = NLMSG_PAYLOAD(nh,sizeof(struct ifinfomsg));
		char *rtaData = NULL;
		int i = 0;
		printf("rta:%p,len:%d\n",rta,rtlen);
		for(rta;RTA_OK(rta,rtlen);rta=RTA_NEXT(rta,rtlen)){
		    printf("rta.rta_len:%d\n",rta->rta_len);
		    printf("rta.rta_type:%d\n",rta->rta_type);
		    printf("payload len:%d\n",RTA_PAYLOAD(rta));
		    rtaData = RTA_DATA(rta);
		    if(rta->rta_type == 3){
			printf("%s \n",rtaData);
		    }else{
			for(i = 0;i < rta->rta_len;i++)
			    printf("%d ",rtaData[i]);
		    }
		    printf("\n");
		}

	    }

	}

	printf("\n");
    }

    close(fd);
    return 0;
}
