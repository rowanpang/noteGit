#!/bin/bash
function Usage() {
    echo './run.sh '
    echo './run.sh -e 'essid' '
}

function argsParser() {
    while [ $# -gt 0 ]; do
	case "$1" in
	    -e)
		tarESSID=$2
		shift
		;;
	    -h)
		Usage
		exit
		;;
	esac
	shift
    done
}

function genCheckScript(){
    cat > $scriptName <<EOF
#!/bin/bash
airodump-ng -a -c $tarChl --essid ${tarESSID} --bssid $tarBSSID --output-format csv  -w $clientLog $ifmon
EOF
    chmod a+x $scriptName
}

function refreshCSV() {
    rm -rf radioCheck-*.csv
    rm -rf outSorted.csv
    airmon-ng start $ifname 

    timeout --foreground 20s airodump-ng -w radioCheck --output-format csv wlp2s0mon
    airmon-ng stop $ifmon
}

function main(){
    [ $UID != 0 ] && echo -e "\033[0;31m------USE ROOT---------\033[0;0m" && exit
    argsParser $@
    refreshCSV
    if [ "$tarESSID" == '' ];then
	if [ -s radioCheck* ];then
	    ./sort.py radioCheck*  | tee outSorted.csv
	fi

    else
	if [ -s radioCheck* ];then
	    ./sort.py -e $tarESSID radioCheck*
	fi
    fi
}

ifname="wlp2s0"
ifmon="${ifname}mon"

main $@
