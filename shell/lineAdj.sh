#!/bin/bash
function lineadj(){
    ln=$1
    lo=$2
    bs=""
    nsp=""

    lb=$ln
    if [ $ln -lt $lo ];then
	((ld=lo-ln))
	while [ $ld -ge 1 ];do
	    nsp="$nsp "
	    ((ld=ld-1))
	done
	echo -en "$nsp"

	lb=$lo
    fi

    while [ $lb -ge 1 ];do
        bs="$bs\\b"
        ((lb=lb-1))
    done
    echo -en "$bs\b\b"
}

sek=-5
echo "60 Seconds Wait!"
echo -en "\e[?25l"
echo -n "One Moment please "
lo=0
while [ $sek -lt 15 ]
do
    bs=""
    echo -n "$sek s"
    ln=${#sek}
    sek=$[$sek+1]
    lineadj $ln $lo
    lo=$ln
    sleep 1
done
echo -e "\e[?25h"
echo "ready!"
