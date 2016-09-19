#!/bin/bash
#=============================================================================
#     FileName: wlandhcp.sh
#         Desc: called by wpa_cli and trigger by wpa_supplicant CONNECTED or DISCONNECTED event
#       Author: Rowan Pang
#        Email: pangweizhen.2008@hotmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2016-03-28 10:56:44
#      History:
#=============================================================================

LOG_FILE=/var/log/wlandhcp.log
#unit k,file block size
LOG_FILE_LIMIT=512
DEBUG="true"
LOG_INIT(){
	fsize=`ls -sk $LOG_FILE | awk '{print $1}'`
	if [ $fsize -gt $LOG_FILE_LIMIT ];then
		echo "" > $LOG_FILE
	fi

	echo "" >> $LOG_FILE
	echo "start----:`date`" >> $LOG_FILE
	echo " EUID----:$EUID" >> $LOG_FILE
}

LOG(){
	if [ "$DEBUG" == "true" ];then
		echo $1 >> $LOG_FILE
	fi
}

ifstatus(){
	wpa_cli status -i $interface >> $LOG_FILE
}

ifdhclient(){
	LOG "ifdhclient"
	dhclient -4 -H $HOSTNAME -1 -q -lf /var/lib/dhclient/dhclient-${interface}.leases -pf /var/run/dhclient-${interface}.pid $interface
}

ifdhclient_release(){
	if [ -f "/var/run/dhclient-${interface}.pid" ]; then
	LOG "ifdhclient_release"
		dhcpid=$(cat /var/run/dhclient-${interface}.pid)
		dhclient -r -lf /var/lib/dhclient/dhclient-${interface}.leases -pf /var/run/dhclient-${interface}.pid ${interface} >/dev/null 2>&1
		retcode=$?
        if [ -f "/var/run/dhclient-${interface}.pid" ]; then
            rm -f /var/run/dhclient-${interface}.pid
            kill $dhcpid >/dev/null 2>&1
        fi  
    fi
}

change_route(){
	LOG "disconnect change route"
	local rip=`ip route | grep 192.168. | awk {'print $1'}`
	local ripdomain=${rip%\.*}	

	LOG "route domain ${ripdomain}"
	ip route add to default via ${ripdomain}.1
}

#-------------------main----------------------------------
LOG_INIT

if [ $# -lt 2 ];then
	LOG "Usage: $0 [INTERFACE] [event]"
	exit 1;
fi

interface=$1
ifevent=$2

LOG "if:$interface"
LOG "reason:$ifevent"

if [ $ifevent == "CONNECTED" ];then
	LOG "connected event"
	ifdhclient
	ifstatus;
elif [ $ifevent == "DISCONNECTED" ];then
	LOG "disconnected event"
	ifdhclient_release
	ifstatus
	change_route
elif [ $ifevent == "DISCONNECTED_RC" ];then
	LOG "disconnected_rc event"
	ifdhclient_release
fi

