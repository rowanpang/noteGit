#!/bin/bash

[ $UID != 0 ] && echo -e "\033[0;31m------USE ROOT---------\033[0;0m" && exit

function argsParser() {
    while [ $# -gt 0 ]; do
	case "$1" in
	    -e)
		tarESSID=$2
		shift
		;;
	esac
	shift
    done
}

#local scriptName="${SSID}-script-checkCli.sh"
function genCheckScript(){
    cat > $scriptName <<EOF
#!/bin/bash

#airmon-ng start $ifname
airodump-ng -a -c $tarChl --essid ${tarESSID} --bssid $tarBSSID --output-format csv  -w $clientLog $ifmon
#airmon-ng stop $ifmon

EOF
    chmod a+x $scriptName
}

tarSSID=""
ifname="wlp2s0"
ifmon="${ifname}mon"

argsParser $@
if [ "$tarESSID" == '' ];then
    rm -rf radioCheck-*.csv
    rm -rf outSorted.csv

    airmon-ng start $ifname 
    timeout --foreground 20s airodump-ng -w radioCheck --output-format csv wlp2s0mon

    if [ -s radioCheck* ];then
	./sort.py radioCheck*  | tee outSorted.csv
    fi
    #airmon-ng stop $ifmon

else
    if [ -s radioCheck* ];then
	target=$(./sort.py -e $tarESSID radioCheck*)
    fi
    tarChl=`echo "$target" | awk '{print $1}'`
    tarBSSID=`echo "$target" | awk '{print $2}'`
    echo $tarBSSID 
    echo $tarESSID
    echo $tarChl
    scriptName="${tarESSID}-script-checkCli.sh"
    clientLog="${tarESSID}-clients"
    genCheckScript
    ./$scriptName
fi
