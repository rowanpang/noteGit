#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <time.h>

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    printf("-----%s-----\n",__func__);
    printf("pkg time:%.2f\n",header->ts.tv_sec + header->ts.tv_usec/1000000.0);
    usleep(20*1000);	    //timer for kernel to prepare the pkg for libpcap 
			    //man 7 packet  PACKET_RX_RING  TP_STATUS_KERNEL
    return;
}

int main(int argc,char** argv)
{
    int ret = 0;
    char *dev, errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program filter;

    char filter_app[] = "tcp";
    bpf_u_int32 mask;	    //mask
    bpf_u_int32 net;	    //ip address

    struct pcap_pkthdr header;
    const u_char *packet;

    int i = 0;

    dev = pcap_lookupdev(errbuf);
    dev = "bridge0";
    printf("Device: %s\n", dev);

    /*pcap_lookupnet(dev,&net,&mask,errbuf);*/
    handle = pcap_open_live(dev,BUFSIZ,1,0,errbuf);

    pcap_compile(handle,&filter,filter_app,0,net);
    pcap_setfilter(handle,&filter);

    /*packet = pcap_next(handle,&header);*/
    /*printf("jacked a packet with length of %d,%d\n",header.len,header.caplen);*/

    /*for (i = 0; i < header.len; i++){*/
	/*printf("%#x ",*packet);*/
	*packet++;*/
    /*}*/
    printf("\n");

    int cnt = pcap_dispatch(handle,3,got_packet,NULL);
    /*int cnt = pcap_loop(handle,3,got_packet,NULL);*/
    printf("jacked %d packets\n",cnt);

    pcap_close(handle);

    return ret;
}
