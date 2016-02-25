#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <error.h>


static int init_hotplug_sock(void)
{
    struct sockaddr_nl snl;
    const int buffersize = 16 * 1024 * 1024;
    int retval;
 
    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    //snl.nl_groups = 10;
    //snl.nl_groups = 0xffffffff;
    snl.nl_groups = 1;
 
    int hotplug_sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (hotplug_sock == -1) {
        printf("error getting socket: %s", strerror(errno));
        return -1;
	}

	/* set receive buffersize */
    setsockopt(hotplug_sock, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
	retval = bind(hotplug_sock, (struct sockaddr *) &snl, sizeof(struct sockaddr_nl));
    if (retval < 0) {
        printf("bind failed: %s", strerror(errno));
        close(hotplug_sock);
        hotplug_sock = -1;
        return -1;
    }

    return hotplug_sock;
}

 

#define UEVENT_BUFFER_SIZE      2048


int main(int argc, char* argv[])
{
         int hotplug_sock = init_hotplug_sock();
		 struct sockaddr_nl nladdr;
		 nladdr.nl_family = AF_NETLINK;
		 nladdr.nl_pid = 0;
         nladdr.nl_groups = 1;
    	 
         char ms[15]={"1_u"};
		 struct iovec iov;
		 
		 struct msghdr msg;
		 memset(&msg,0,sizeof(msg));
		 msg.msg_name = &nladdr;
		 msg.msg_namelen = sizeof(nladdr);
         msg.msg_iov = &iov;
		 msg.msg_iovlen = 1;
 		 msg.msg_control = 0;
 		 msg.msg_controllen = 0;
 		 msg.msg_flags = 0;
	
		#define MAX_PAYLOAD 1024
		struct nlmsghdr *nlh = NULL;
		nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
		memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
		nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
		nlh->nlmsg_pid = getpid();
		nlh->nlmsg_flags = 0;
		strcpy(NLMSG_DATA(nlh), "Hello------");
		iov.iov_base = (void *)nlh;
		iov.iov_len = nlh->nlmsg_len;	
	
         while(1){
			   char buf[UEVENT_BUFFER_SIZE*2] = {0};
			   recv(hotplug_sock, &buf, sizeof(buf), 0); 
			   printf("%s\n", buf);
			   usleep(100);
				
			   int err = sendmsg(hotplug_sock,&msg,0);
			   if(err == -1){	
				   printf("error send msg,case %s\n",strerror(errno));
			   }
         }

         return 0;
}
