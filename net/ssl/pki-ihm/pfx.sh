#!/usr/bin/bash

function Usage() {
    echo "$0 mailAuthor"
    echo "    exp: $0 163"
}

if [ $# -lt 1 ];then
    Usage
    exit
fi

prefixs=$@

caCfg="root.cfg"
caPrefix=$(grep output_password $caCfg | awk 'BEGIN{FS = "="};{print $2}')
rootCA="`echo $caPrefix`.cer"

for prefix in $prefixs;do
    echo "---now prefix: $prefix"
    prefix=${prefix%.*}
    prefix=${prefix#cfg-}
    if ! [ -s "$prefix.csr" ];then
	./csr.sh $prefix
    fi

    if ! [ -s "$prefix.cer" ];then
	./ca.sh $prefix
    fi

    echo ""
    echo ""
    echo "---gen $prefix.pfx---"
    openssl pkcs12 -export -inkey $prefix.priv			\
	    -name "$prefix-${rootCA%.*}" -in $prefix.cer	\
	    -caname "self-${rootCA%.*}" -certfile $rootCA	\
	    -out $prefix.pfx
done
