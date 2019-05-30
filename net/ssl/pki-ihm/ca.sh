#!/usr/bin/bash

function Usage() {
    echo "$0 mailAuthor"
    echo "    exp: $0 163"
}

if [ $# -lt 1 ];then
    Usage
    exit
fi

prefix=${1%.*}
prefix=${prefix#cfg-}
cfg="ca.cfg"

caCfg="root.cfg"
caPrefix=$(grep output_password $caCfg | awk 'BEGIN{FS = "="};{print $2}')

if ! [ -d cadir ];then
    mkdir cadir
    mkdir cadir/certs
    mkdir cadir/crl
    mkdir cadir/newcerts

    touch cadir/index.txt
    touch cadir/index.txt.attr
    echo 05 > cadir/serial
fi

openssl ca -config $cfg -in $prefix.csr -out $prefix.cer		\
	   -cert $caPrefix.cer -keyfile $caPrefix.priv -key $caPrefix	\
	   -notext -batch
