#!/bin/sh

program=$0

function Usage(){
	echo "need param"
	echo "Usage: $program 'domain'"
}

#check param
if [ $# -lt 1 ];then
	Usage
	exit -1;
fi	
domain=${1%%.*}
xmlConfig=${domain}.xml
vncPort=""

#confirm vncPort for domain throw /proc/$pid/cmdline
function gotDomainVncPort(){
	local pid port pids
	local pidPort=( )
	oldIFS="$IFS"
	IFS=$'\n'
	allVnc=`netstat -nptl 2>/dev/null | grep ":59[0-9][0-9]"`
	for vnc in $allVnc;do
		unset pid port
		#echo $vnc
		pid=$(echo $vnc | awk '{print $7}' | sed 's#\([0-9]\+\)/.*#\1#')
		port=$(echo $vnc | awk '{print $4}' | sed 's/.*:\(59[0-9][0-9]\)/\1/')
		pids="$pid $pids" 
		pidPort[$pid]=$port	
	done

	IFS="$oldIFS"
	#trim ^\s and \s$
	pids=$(echo $pids | sed 's/^\s\?\([0-9]\+\)\s\?$/\1/')
	for pid in $pids;do
		#echo $pid
		count=$(cat /proc/$pid/cmdline | grep "domain-[0-9]\+-$domain" -c)
		if [ $count -gt 0 ];then
			echo ${pidPort[$pid]}
			match="yes"
			break;
		fi
	done
	[ $match ] || echo "not found vnc port for $domain" && exit -1
}

function create(){
	
	virsh create $xmlConfig

	vncPort=$(gotDomainVncPort)
	echo "vncPort: $vncPort"
	vncviewer :$vncPort   >/dev/null 2>&1
	
	virsh destroy $domain
}
#main
echo "domain: $domain"
echo "xmlConfig: $xmlConfig"
i3-msg 'floating toggle ;resize set 270 200;move position 10 20' >/dev/null 2>&1
create
i3-msg 'floating toggle'  >/dev/null 2>&1
