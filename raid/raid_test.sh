#!/bin/bash

##########################################################
#	author	:pangwz	pangweizhenbj@inspur.com 
#	date	:2015-3-25
#	describe:Used for software raid test-->mdadm
##########################################################


#-----------debug info dynamic
debugEnable=	#true
function dyDEBUG()
{
	if [ "$debugEnable" = true ];then
		$@
	fi
}

function permission_check()
{
	if [ $USER = root -o $USER = ROOT ];then
		dyDEBUG echo "permission ok"	
	else
		echo -e "\033[0;31m"
		echo "----------------Exit abnormal-----------"
		echo "permission ng,need to be root,please change to root and exec again!!!"
		echo -e "\033[0m"
		exit -1
	fi
}

identify=`date +%Y%m%d%H%M%S`
dyDEBUG echo "file identify:$identify"

TOTAL_HD_NUM=5
function make_hd_img()
{
	local i=0
	while [ $i -lt $TOTAL_HD_NUM ];do
		dd if=/dev/zero of=./hd_${i}_${identify} bs=1024 count=40960
		dyDEBUG echo "dd make hd for $i"
		let "i += 1"
	done
}


function del_hd_img()
{
	local i=0
	while [ $i -lt $TOTAL_HD_NUM ];do
		if [ -e ./hd_${i}_${identify} ];then
			rm -f ./hd_${i}_${identify}
			dyDEBUG echo "del hd_img hd_${i}_${identify}"
		fi		
		let "i += 1"
	done
}
function loop_valid()
{
	LOOP_START=`losetup	-f`
	dyDEBUG echo "loop start name:$LOOP_START"
	LOOP_START_NUM=${LOOP_START:9}
	dyDEBUG echo "loop start num:$LOOP_START_NUM"
}

function loop_setup()
{
	local i=0
	local loop_dev_name=
	while [ $i -lt $TOTAL_HD_NUM ];do
		loop_dev_name=`losetup -f`
		losetup ${loop_dev_name} ./hd_${i}_${identify}
		eval HD_LOOP_TO_$i=${loop_dev_name}
		dyDEBUG echo "losetup for loop $loop_dev_name hd $i"
		let "i += 1"
	done

	dyDEBUG losetup -a
}

function unloop()
{
	local i=0
	local loop_dev_name=
	while [ $i -lt $TOTAL_HD_NUM ];do
		#eval echo "$"HD_LOOP_TO_$i""
		eval loop_dev_name="$"HD_LOOP_TO_${i}""
		losetup -d $loop_dev_name 
		dyDEBUG echo "losetup unloop $loop_dev_name hd $i"  		
		let "i += 1"
	done
	
	dyDEBUG losetup -a
}

function raid_0()
{
	dyDEBUG echo -e "raid_0 \c"
	dyDEBUG echo "use $HD_LOOP_TO_0 $HD_LOOP_TO_1"
	mdadm --create --auto=yes /dev/md0 --level=0 --raid-devices=2  $HD_LOOP_TO_0 $HD_LOOP_TO_1

	local mount_point=/mnt/raid_$identify 
	dyDEBUG echo "mount point:$mount_point"
	mkfs.ext4 /dev/md0
	mkdir $mount_point
	mount /dev/md0 $mount_point

	touch $mount_point/raid_test.txt
	if [ -e $mount_point/raid_test.txt ];then
		raid_0_test_str="ok"
	else
		raid_0_test_str="ng"
	fi
	
	dyDEBUG echo -e "\033[0;31mRAID0_result:$raid_0_test_str\033[0m"
	
	umount $mount_point 
	rm -rf $mount_point 

	mdadm --stop /dev/md0
	mdadm --zero-superblock $HD_LOOP_TO_0 
	mdadm --zero-superblock $HD_LOOP_TO_1 
	
}

function raid_com()
{
	local lv=$1	
	dyDEBUG echo -e "\033[0;31mRAID_lv:$lv:\033[0m"

	if [ $lv = 0 ];then
		mdadm --create --auto=yes /dev/md0 --metadata=default --level=0 --raid-devices=2  $HD_LOOP_TO_0 $HD_LOOP_TO_1
		dyDEBUG echo "use $HD_LOOP_TO_0 $HD_LOOP_TO_1"
	elif [ $lv = 1 ];then
		mdadm --create --auto=yes /dev/md0 --metadata=0.90 --force --level=$lv --raid-devices=2  $HD_LOOP_TO_0 $HD_LOOP_TO_1
		dyDEBUG echo "use $HD_LOOP_TO_0 $HD_LOOP_TO_1"
	elif [ $lv = 5 -o $lv = 10 -o $lv = 6 ];then
		yes | mdadm --create --auto=yes /dev/md0 --level=$lv --force --raid-devices=4 --spare-devices=1 $HD_LOOP_TO_0 $HD_LOOP_TO_1 $HD_LOOP_TO_2 $HD_LOOP_TO_3 $HD_LOOP_TO_4
		dyDEBUG echo "use $HD_LOOP_TO_0 $HD_LOOP_TO_1 $HD_LOOP_TO_2 $HD_LOOP_TO_3 $HD_LOOP_TO_4"
	else 
		echo "error lv num $lv"
	fi

	local mount_point=/mnt/raid_$identify 
	dyDEBUG echo "mount point:$mount_point"
	mkfs.ext4 /dev/md0
	mkdir $mount_point
	mount /dev/md0 $mount_point

	touch $mount_point/raid_test.txt
	if [ $lv != 0 ];then
		mdadm /dev/md0 --fail $HD_LOOP_TO_0
		dyDEBUG mdadm --detail /dev/md0		
	fi	
	usleep 100	
	if [ -e $mount_point/raid_test.txt -a -e /dev/md0 ];then
		eval raid_${lv}_test_str="ok"
	else
		eval raid_${lv}_test_str="ng"
	fi


	dyDEBUG echo -e "\033[0;31m\c"
	dyDEBUG echo -e "RAID_${lv}_result:\c"
	dyDEBUG eval echo -e "$"raid_${lv}_test_str""
	dyDEBUG echo -e "\033[0m\c"
		
	umount $mount_point 
	usleep 100
	rm -rf $mount_point 

	mdadm --stop /dev/md0
	usleep 100
	
	if [ $lv = 1 -o $lv = 0 ];then
		mdadm -v --zero-superblock $HD_LOOP_TO_0 
		mdadm -v --zero-superblock $HD_LOOP_TO_1 
	elif [ $lv = 5 -o $lv = 10 -o $lv = 6 ];then
		mdadm -v --zero-superblock $HD_LOOP_TO_0 
		mdadm -v --zero-superblock $HD_LOOP_TO_1 
		mdadm -v --zero-superblock $HD_LOOP_TO_2 
		mdadm -v --zero-superblock $HD_LOOP_TO_3 
	else
		echo "error !!!!!------"
	fi	
}

function col_result
{
	echo -e "\033[0;31m\c"
	echo "-----------result------------"
	for lv in 0 1 5 6 10 
	do
		echo -e "RAID_${lv}_result:	\c"
		eval echo -e "$"raid_${lv}_test_str""
	done
	echo "-----------end------------"
	echo -e "\033[0m\c"
}


permission_check
make_hd_img
loop_setup

raid_com 0
raid_com 1 
raid_com 5 
raid_com 10
raid_com 6 

unloop
del_hd_img

col_result
