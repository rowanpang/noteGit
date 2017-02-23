#!/bin/bash

host=$1
port=$2
`tcping $host $port -q -t 1`
ret=$?
#if [ $ret -eq 0 ];then
    #echo "ok" 
#else 
    #echo "fail"
#fi

[ $ret -eq 0 ] && echo "ok" || echo "fail"
