#!/bin/sh -e
LOG=/var/log/udev_info.log
#LOG=/dev/null

KERNEL_NAME=$1
KERNEL_NUM=$2

echo $0  >> $LOG
echo "$ACTION,$SUBSYSTEM,$KERNEL_NAME,$KERNEL_NUM,$DEVPATH" >> $LOG
