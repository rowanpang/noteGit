#!/bin/bash

if [ $# -eq 0 ];then
    echo "usage: $0 sched_method"
    exit
fi

sched_method=$1
./rttest $sched_method 99 &
sleep 0.1
./rttest $sched_method 70 &
sleep 0.1
./rttest $sched_method 70 &
sleep 0.1
./rttest $sched_method 70 &
sleep 0.1
./rttest $sched_method 50 &
sleep 0.1
./rttest $sched_method 30 &
sleep 0.1
./rttest $sched_method 10 &
