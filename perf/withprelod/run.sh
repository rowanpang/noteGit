#!/bin/bash

if [ $# -gt 0 ];then
    export LD_PRELOAD="./libmsg.so"
fi

echo "env:$LD_PRELOAD"

./main
