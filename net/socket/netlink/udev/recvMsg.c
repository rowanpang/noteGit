#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NL_MAX_PAYLOAD 8192

int main (int argc,char **argv) {
    int nl_socket;
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr* nlh;
    struct msghdr msg;
    struct iovec iov;

    // Prepare source address
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();
    src_addr.nl_groups = 1;

    // Prepare netlink message
    nlh = (struct nlmsghdr*) malloc(NLMSG_SPACE(NL_MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(NL_MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(NL_MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    iov.iov_base = (void *) nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    nl_socket = socket(AF_NETLINK, SOCK_DGRAM,      NETLINK_KOBJECT_UEVENT);
    if (nl_socket < 0) {
        printf("Failed to create socket for DeviceFinder");
        exit(1);
    }

    bind(nl_socket, (struct sockaddr*) &src_addr, sizeof(src_addr));

    while (1) {
        int r = recvmsg(nl_socket, &msg, 0);
        /*printf("%i\n", r);*/
	printf("%s\n",NLMSG_DATA(nlh));
        if (r < 0) {
            perror("");
        }
    }
}
