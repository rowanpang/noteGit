#include <stdio.h>
#include <pcap.h>

int main(int argc,char** argv)
{
    int ret = 0;
    char *dev, errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    struct bpf_program filter;

    char filter_app[] = "port 10800";
    bpf_u_int32 mask;	    //mask
    bpf_u_int32 net;	    //ip address

    struct pcap_pkthdr header;
    const u_char *packet;

    int i = 0;

    dev = pcap_lookupdev(errbuf);
    printf("Device: %s\n", dev);

    pcap_lookupnet(dev,&net,&mask,errbuf);
    handle = pcap_open_live(dev,BUFSIZ,1,0,errbuf);

    pcap_compile(handle,&filter,filter_app,0,net);
    pcap_setfilter(handle,&filter);

    packet = pcap_next(handle,&header);
    printf("jacked a packet with length of %d,%d\n",header.len,header.caplen);

    for (i = 0; i < header.len; i++){
	printf("%#x ",*packet);
	packet++;
    }
    printf("\n");

    pcap_close(handle);

    return ret;
}
