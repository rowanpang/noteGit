#!/bin/bash

if ! grep -q "self" ./test.txt;then
    echo "grep ok";
else
    echo "grep ng"
fi

if [ 0 ];then
    echo "0 is true"
else
    echo "0 is false"
fi

if [ 1 ];then
    echo "1 is true"
else
    echo "1 is false"
fi

if [ $? -eq 0 ];then
    echo "0 is true"
else
    echo "0 is false"
fi
