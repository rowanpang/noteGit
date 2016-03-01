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


static int init_hotplug_sock(void)
{
    struct sockaddr_nl snl;
    const int buffersize = 16 * 1024 * 1024;
    int retval;
 
    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
	//group 0b0010 used by udev for broadcat,search NETLINK_KOBJECT_UEVENT
    snl.nl_groups = 0b0001;
 
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
	char buf[UEVENT_BUFFER_SIZE*2] = {0};
	int size = 0;
	int i,pos = 0;
	while(1){
		memset(buf,0,sizeof(buf));
		size = recv(hotplug_sock, buf, sizeof(buf), 0); 
		printf("size:%d,head:%s\n",size,buf);

		i = strlen(buf)+1;	
		for(; i<size -2 ;){
			printf("pos:%d,%s\n",i,&buf[i]);
			i += (strlen(&buf[i]) + 1);
		}
		printf("\n");

		usleep(100);
	}

	return 0;
}
