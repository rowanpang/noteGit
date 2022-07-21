#!/bin/bash

##########################################################
#	author	:pangwz	pangweizhenbj@inspur.com 
#	date	:2015-3-5
#	describe:Used for test Unicode Support,test result display at the end!!!
##########################################################



#-----------debug info dynamic
debugEnable=	#true
function dyDEBUG()
{
	if [ "$debugEnable" = true ];then
		$@
	fi
}

function wchar_test()
{
	if [ -e "./$wchar_bin" -a -x "./$wchar_bin" ];then
		./$wchar_bin
	else
		echo -e "\033[0;31m"
		echo "----------wchar test exit abnormal-----------"
		echo "File not exist or user didn't have exec permission,please check"
		echo -e "\033[0m"
		exit 1
	fi 
	
	wchar_result=$?
	dyDEBUG echo "wchar_result: $wchar_result"
	if [ $wchar_result -eq 0 ];then
		wchar_result_str="ok"
		echo "ChineseSimplify Out?[y/n]"
		read usr_Answer
		while [ "$usr_Answer" != "y" -a "$usr_Answer" != "n" ]
		do
			echo -e "ChineseSimplify Out?[y/n]\c"
			read usr_Answer 
		done
		
		dyDEBUG echo "answer:$usr_Answer"
		if [ "$usr_Answer" = "n" ];then
			wchar_result_str="ng"
		fi
	else
		wchar_result_str="ng"
	fi 
}

function char_conv()
{
	#"中":Unicode 0x4E2D
	#"文":Unicode 0x6587
	
	echo "中文" > file_utf.txt
	iconv -f utf8 -t utf-16 -o file_ucs.txt ./file_utf.txt
	out=$(xxd -g 1 ./file_ucs.txt | awk '{printf $5$4 $7$6 $8$9"\n"}')
	#echo $out
	if [ $out = "4e2d65870a00" ];then
		conv_result_str="ok"
		dyDEBUG echo "ok"
	else
		conv_result_str="ng"
		dyDEBUG echo "ng"
	fi
	
	rm file_utf.txt
	rm file_ucs.txt
}

fat_img_name=fat_tmp.img
fat_fs_name=fat_fs
filebak_identify=`date +%Y%m%d%H%M%S`

function fat_loop_filebak()
{
	dyDEBUG echo "filebak_identify:$filebak_identify"
	if [ -e ./$fat_img_name ];then
		mv ./$fat_img_name ./${fat_img_name}_$filebak_identify
	fi
		
	if [ -e ./$fat_fs_name ];then
		mv ./$fat_fs_name ./${fat_fs_name}_$filebak_identify
	fi
}

function fat_loop_filerestore()
{
	dyDEBUG echo "${fat_img_name}_${filebak_identify}"

	if [ -e ${fat_img_name}_$filebak_identify ];then
		dyDEBUG echo "restore:${fat_img_name}_$filebak_identify"
		cp ./${fat_img_name}_$filebak_identify ./$fat_img_name	
	fi
	
	if [ -e ${fat_fs_name}_$filebak_identify ];then
		dyDEBUG echo "restore:${fat_fs_name}_$filebak_identify"
		cp ./${fat_fs_name}_$filebak_identify ./$fat_fs_name	
	fi	
}

function fat_loop()
{
	Cur_usr=$USER
	dyDEBUG echo "cur_user: A${Cur_usr}Z"
	if [ "$Cur_usr" = root -o "$Cur_usr" = ROOT ];then
		dyDEBUG echo " permission ok"
		fat_loop_filebak
		loop_name=$(losetup -f) 
		dd if=/dev/zero of=./$fat_img_name bs=1024 count=4096
		if [ -e ./$fat_tmp_name ];then
			dyDEBUG echo "image creat ok"
		
			losetup $loop_name ./$fat_img_name
			mkfs.vfat $loop_name
		
			mkdir ./$fat_fs_name
			mount -t vfat -o utf8 $loop_name ./$fat_fs_name
			
			mkdir ./$fat_fs_name/a中文目录z
			touch ./$fat_fs_name/a中文文件z.txt
			
			if [ -e ./$fat_fs_name/a中文目录z -a -e ./$fat_fs_name/a中文文件z.txt ];then
				fat_result_str="ok"	
			else
				fat_result_str="ng"	
			fi

			ls -lia ./$fat_fs_name
			umount ./$fat_fs_name
			losetup -d $loop_name
		
		#delete new file	
			rm -rf ./$fat_fs_name
			rm ./$fat_img_name
	
		else
			echo "------error----------"
		fi
		fat_loop_filerestore	
	else
		echo -e "\033[0;31m"
		echo "----------------Test exit abnormal-----------"
		echo "permission ng,need to be root,please change to root and exec again!!!"
		echo -e "\033[0m"
		exit 1
	fi 
}

function result_coll()
{
	echo "-----------------------"
	echo -e "wchar support test result: \033[0;31m$wchar_result_str\033[0m"
	echo -e "conv support test result: \033[0;31m $conv_result_str\033[0m"
	echo -e "fat support test result: \033[0;31m $fat_result_str\033[0m"
	echo "-----------------------"
}


#-----------main--------
wchar_bin="wchar"
wchar_result=
wchar_result_str=
conv_result_str=
fat_result_str=

echo "Unicode support start"
wchar_test
char_conv
fat_loop

result_coll

