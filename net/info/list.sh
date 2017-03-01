#!/bin/sh

ifnames=`ip link | awk 'BEGIN { FS=": "}; NR%2 != 0 {print $2}'`
for ifname in $ifnames;do 
    ifIp=`ip addr show $ifname  | awk '/inet / {print $2}'`;
    ifMac=`ip addr show $ifname  | awk '/link\/ether/ {print $2}'`;
    echo -e "$ifname:\t\t$ifMac \t $ifIp";
done
