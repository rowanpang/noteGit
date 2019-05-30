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
echo "--generate $prefix.csr use cfg-$prefix.cfg--"
openssl req -new -subject -config cfg-$prefix.cfg -out $prefix.csr \
	    -keyout $prefix.priv
