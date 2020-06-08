#!/bin/sh
file=log.psinfo.txt
echo > $file

for((i = 0; i < 40; i++));do
    ps -C rttest -o pid,pri,cmd,time,psr >> $file 2>&1
    echo "" >> $file
    sleep 2;
done
