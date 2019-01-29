#!/bin/bash
function lineadj(){
    ln=$1
    bs=""
    nsp=""
    while [ $ln -ge 1 ];do
        bs="$bs\\b"
	nsp="$nsp "
        ((ln=ln-1))
    done
    echo -en "$bs\b\b"
    echo -en "$nsp  "
    echo -en "$bs\b\b"
}

sek=-5
echo "60 Seconds Wait!"
echo -n "One Moment please "
lo=0
while [ $sek -lt 15 ]
do
    bs=""
    echo -n "$sek s"
    ln=${#sek}
    sleep 1
    sek=$[$sek+1]
    lineadj $ln $lo
    lo=$ln
done
echo
echo "ready!"


