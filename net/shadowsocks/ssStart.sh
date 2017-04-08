#!/bin/bash

function parserPwdIp(){
    pwd=$1
    ip=$2
}

function parserConfs(){
    method=$1
    pwdIp=$2
    port=$3
    IFS='@'
    parserPwdIp $pwdIp
}


function starfromQR(){
    qrimg=$1
    quiet="-q"
    wget $quiet -O $qrimg http://xyz.ishadow.online/img/qr/${qrimg}
    info=$(zbarimg $quiet $qrimg)
    base64=${info#QR-Code:ss://}
    confs=$(echo $base64 | base64 -d)
    oifs=$IFS
    IFS=':'
    parserConfs $confs
    IFS="$oifs"
    echo $qrimg > curConf.txt
    echo $method $pwd $ip $port >> curConf.txt
    sslocal $quiet -d restart -s $ip -p $port -k $pwd -m $method \
	    --pid-file ./ss.pid --log-file ss.log
}

function checkOk(){
    `timeout 15 curl --insecure --silent --proxy socks5h://127.0.0.1:1080 https://twitter.com > /dev/null`
    ret=$?
    [ $ret -eq 0 ] && echo "ok" || echo "ng"
}

for co in sg us jp;do
    for index in {a..c};do
	tmpFile="${co}${index}.png"
	#tmpFile="sga.png"

	echo "current $tmpFile"
	starfromQR $tmpFile
	[ $(checkOk) == "ok" ] && break 2;
    done
done
