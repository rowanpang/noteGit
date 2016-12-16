#!/bin/sh
#howToUse:
	#1,configIterm need Modify
		#xmlTemplate
		#domainSavedDir
		#imgSizeWhenAutoCreate='25G'
	#2,./vmStart.sh *.xml or *.iso

#howTodebug
	

#depend on
	#1,qemu-img to gen imgDisk
	#2,awk/sed/python
	#3,netstat auto find vncport
	#4,i3-msg adjust win
	#5,uuidgen,auto gen uuid

program=$0
DEBUG="yes"

function Usage(){
	echo "need param"
	echo "Usage: $program 'domain'"
}
function verbose(){
	[ $DEBUG == "yes" ] && {  $@; }
}
TAB="-e \t"
#check param
if [ $# -lt 1 ];then
	Usage
	exit -1;
fi	

domain=${1%.*}
workdir="$PWD/"
xmlConfig=${domain}.xml
xmlTemplate='/home/pangwz/test/kvm/fw24.xml'

imgSizeWhenAutoCreate='25G'
imgDisk=${workdir}${domain}.img
logFile="${workdir}serial-${domain}.log"
domainIso=${workdir}${domain}.iso

vncPort=""
domainSavedDir="/home/pangwz/.config/libvirt/qemu/save/"
domainSaved=$domainSavedDir$domain.save
domainFSDir=${domain}FS/

function domainCreate(){
	local ret;
	if [ -r $domainSaved ];then
		verbose echo "restore domain $domainSaved"
		#echo -e "\033[1;32m restore domain $domainSaved && delete it\033[0m"
		virsh restore $domainSaved
		ret=$?
		rm -rf $domainSaved
	else
		verbose echo "create domain $xmlConfig"
		#echo -e "\033[1;32m create domain $xmlConfig \033[0m"
		virsh create $xmlConfig
		ret=$?
	fi

	if [ $ret -eq 0 ];then
		echo -e "\033[1;32mdomain $xmlConfig up\033[0m"
	else
		echo -e "\033[1;31mdomain $xmlConfig up failed,exit -1\033[0m"
		exit -1
	fi
}

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
		#echo "match pid: $pid"
		count=$(cat /proc/$pid/cmdline | grep "domain-[0-9]\+-${domain:0:20}" -c)
		if [ $count -gt 0 ];then
			echo ${pidPort[$pid]}
			match="yes"
			break;
		fi
	done
	[ $match ] || { echo -e "\033[1;31m\t\terror!!!not found vnc port for $domain,exit -2\033[0m" && exit -2; }
}

function vncViewer(){
	vncPort=$(gotDomainVncPort)
	echo "vncPort: $vncPort"
	vncviewer :$vncPort   >/dev/null 2>&1

	virsh destroy $domain
}

function getCurWorkSpace(){
	pyName=/tmp/getFocusWorkSpace.py
cat << EOF > $pyName
#!/bin/env python

import sys
import os
import json

jsonStr=os.popen('i3-msg -t get_workspaces').readline()
dicts = json.loads(jsonStr)                                                             
for d in dicts:
    if d['focused'] == True:
        print d['num']
EOF

	chmod a+x $pyName
	curWorkSpace=$($pyName)
	echo $curWorkSpace
	rm $pyName
}

function checkXml(){
	[ -f $xmlConfig ] && return
	verbose echo "auto gen $xmlConfig"
	cp $xmlTemplate $xmlConfig
	local uuid=$(uuidgen) 
	verbose echo "$TAB random uuid\t:$uuid"
#kvm uuid
    sed -i "s#<uuid>\S\+</uuid>#<uuid>$uuid</uuid>#" $xmlConfig
	#sed -i "#<uuid>\S\+</uuid># d" $xmlConfig
#kvm name	
	sed -i "s#<name>\S\+</name>#<name>$domain</name>#" $xmlConfig
#kvm source file
	sed -i "s#\(\s\+<source file=\)\s*\S\+.img'/>#\1\'$imgDisk\'/>#" $xmlConfig 
	sed -i "s#\(\s\+<source file=\)\s*\S\+.iso'>#\1\'$domainIso\'>#" $xmlConfig
	sed -i "s#\(\s\+<source path=\)\s*\S\+.log'/>#\1\'$logFile\'/>#" $xmlConfig
#kvm vnc port autoport
	#skip
#kvm mac
	local macStr=$(echo `sed -n '/<mac address/p' $xmlConfig`)
	macStr=${macStr##\<mac address=\'}
	macStr=${macStr%%\'/>}
	local macHex="0x$(echo $macStr | sed -n 's/://gp')"
	verbose echo "$TAB oldmacHex\t:$macHex"
	local newMac=$(awk "BEGIN{num=strtonum($macHex);printf(\"%X\n\",num+$RANDOM)}")
	verbose echo "$TAB newMacHex\t:$newMac"
	newMacStr=$(echo $newMac | sed -r 's/^([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})([0-9a-fA-F]{2})/\1:\2:\3:\4:\5:\6/')
	verbose echo "$TAB newMacStr\t:$newMacStr"
	sed -i "s#\(\s\+<mac address=\)\s*\S\+'/>#\1\'$newMacStr\'/>#" $xmlConfig
}

function checkImg(){
	[ -f $imgDisk ] && return
	verbose echo "auto create $imgDisk" 
	qemu-img create $imgDisk $imgSizeWhenAutoCreate
}

function check(){
	checkImg && checkXml
}

#main
check || exit
#exit
curWS=$(getCurWorkSpace)
echo "domain: $domain"
echo "xmlConfig: $xmlConfig"
echo "curWorkSpace: $curWS"
#topleft 10 20
#top right 1330 10
domainCreate
i3-msg 'floating toggle ;resize set 270 200;move position 1330 00' >/dev/null 2>&1
vncViewer

timeWait=0
sliceWait=1
while [ 0 ];do
	hereWS=$(getCurWorkSpace)
	if [ $hereWS -eq $curWS ];then
		break
	else
		sleep $sliceWait
		let timeWait+=$sliceWait
		echo "had wait for focus: $timeWait seconds"
	fi
done

i3-msg 'floating toggle'  >/dev/null 2>&1
