#!/bin/sh
#LOG=/var/log/udev_disk_auto_mount.log
LOG=/dev/null
echo $0 >> $LOG

KERNEL_NAME=$1
TMP_DEV_NAME=$2
DISK_LABEL=`/sbin/blkid -p $TMP_DEV_NAME | awk '{print $2}' | sed 's/LABEL=\"\(\S\+\)\"/\1/'`
MNT_DIR=
IDENTIFY=`/bin/date +%Y%m%d%H%M%S`

echo "$ACTION" >> $LOG
echo "tempdev:$TMP_DEV_NAME" >> $LOG

if [ "$ACTION" == "add" ];then
	MNT_DIR=/media/$DISK_LABEL;
	if [ -e $MNT_DIR ];then
		MNT_DIR=/media/${DISK_LABEL}-$IDENTIFY;
	fi
	echo "mnt dir:$MNT_DIR" >> $LOG

	mkdir $MNT_DIR;
	/bin/mount $TMP_DEV_NAME $MNT_DIR;

	echo "after add" >> $LOG
elif [ "$ACTION" == "remove" ];then
	echo "in remove" >> $LOG
	MNT_DIR=`/bin/mount | /bin/grep $KERNEL_NAME | awk '{print $3}'`
	#echo `basename $MNT_DIR` >> $LOG
	echo "mnt dir:$MNT_DIR" >> $LOG

	/bin/umount $MNT_DIR;
	rm -r $MNT_DIR;

	echo "after remove" >> $LOG
fi

sleep 30
