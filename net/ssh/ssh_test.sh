#!/bin/bash

##########################################################
#	author	:pangwz	pangweizhenbj@inspur.com 
#	date	:2015-3-5
#	describe:Use for ssh password and publickey Authentication test
#				test result display at the end!!!
##########################################################



#-----------debug info dynamic
debugEnable=	#true
function dyDEBUG()
{
	if [ "$debugEnable" = true ];then
		$@
	fi
} 

function ssh_func_test()
{
	if ssh -V > /dev/null 2>&1;then
		dyDEBUG	echo "ssh ok"
	else
		echo "ssh error,please check system,exit"
		exit 1
	fi
}

function expect_func_test()
{
	if (which expect > /dev/null 2>&1);then
		dyDEBUG echo "expect ok"
	else
		echo "expect error"

	fi			
}

function ssh_Authen_user_decide()
{
	echo -e "ssh will use user:\033[0;31m$ssh_Authen_user,\033[0m\c"
	echo "Yes:ok...No:chang user. Your choice?"
	read ANS
	while [ "$ANS" != y -a "$ANS" != Yes -a "$ANS" != yes -a "$ANS" != "n" -a "$ANS" != "No" -a "$ANS" != "no" ]
	do
		echo -e "Make right choice??? Yes:ok...No:chang user by yourself: \c"
		read ANS
	done
	
	case $ANS in 
	y|Y|yes|Yes) 
		dyDEBUG echo "Your choice is Yse,use user $ssh_Authen_user" 
	;;
	n|N|No|no) 
		echo "Your decide to change user,please input the user name"
		read ssh_Authen_user
		while [ -z "$ssh_Authen_user" ]
		do
			echo -e "Input a user name:\c"
			read ssh_Authen_user
		done
		dyDEBUG echo "You use \"$ssh_Authen_user\" as user name"
	;;
	
	*) "echo Your choice error,exit 1"
	
	esac

	echo -e "\033[0;31mDecide use user $ssh_Authen_user\033[0m"
}	


function ssh_Authen_user_pwd_get()
{
	ssh_Authen_user_pwd=$ssh_Authen_user
	echo -e "Please \033[0;31mInput pwd \033[0mfor user $ssh_Authen_user[,default is $ssh_Authen_user_pwd]"
	IFS_OLD="$IFS"
	IFS=""
	read ssh_Authen_user_pwd
	while [ -z "$ssh_Authen_user_pwd" ]
	do
		echo -e "Input password for $ssh_Authen_user:\c"
		read ssh_Authen_user_pwd
	done

	IFS=$IFS_OLD
}


function ssh_Authen_method_choice()
{
	ssh_Authen_method_force_default=	#true	#true to enable
	echo -e "avaiable method,please make your choice:\c"

	if [ "$ssh_Authen_method_force_default" = true ];then 
		echo "[default for all:password and pbulickey]"
	else
		echo ""
	fi

	echo "	1,password"
	echo "	2,public key"
	echo "	3,Authen all"
	
	if [ ! "$ssh_Authen_method_force_default" = true ];then 
		read ssh_Authen_method
	fi

	#if didn't need default,just disable if condition
	if [ -z "$ssh_Authen_method" -a "$ssh_Authen_method_force_default" = true ];then
		dyDEBUG echo "-in default produce --"	
		ssh_Authen_method=3
	fi

	while [ "$ssh_Authen_method" != 1 -a "$ssh_Authen_method" != 2 -a "$ssh_Authen_method" != 3 ]
	do
		echo -e "Make choice 1,password/2,public key/3,Authen all:\c"
		read ssh_Authen_method
	done

	dyDEBUG	echo "-------$ssh_Authen_method------"
	case $ssh_Authen_method in
	1)
		ssh_Authen_method_str="password"
	;;

	2)
		ssh_Authen_method_str="public key"
	;;

	3)
		ssh_Authen_method_str="Authen all"
	;;

	*)
		echo "error choice exit 1"
	;;
	
	esac	

	echo -e "choice method is:\033[0;31m$ssh_Authen_method $ssh_Authen_method_str\033[0m"
}

	#spawn ssh pangwz@localhost
function ssh_Authen_method_password()
{
	echo -e "\033[0;31m--------pssword Authen test begin--------\033[0m"
	ssh_Authen_user_pwd_get
expect << eof
	spawn ssh $ssh_Authen_user@localhost
	expect { 
		"*(yes/no)?" { send "yes\n";exp_continue; }
		"*password:" { send "$ssh_Authen_user_pwd\n";send_user "input pwd" }
	}
	expect { 
		"Permission denied*" { send_user "pwd_error\n";exit 1 }
		"*$ " { send_user "login ok\n";exit 0 }
	}
	expect timeout {exit 1}
	
	#timeout {exit 1}
	#expect "Permission denied*" {exit 1}
	#expect "*~]$"
	#send_user "login_ok"
	#expect eof
	
	interact
eof
	pwdresult=$?	
	dyDEBUG	echo "pwd last result $pwdresult"
	if [ $pwdresult -eq 0 ];then
		ssh_Authen_password_result=ok	
	else
		ssh_Authen_password_result=ng
	fi

	echo -e "\033[0;31m--------pssword Authen test end --------\033[0m"
}

keyfilebak_identify=`date +%Y%m%d%H%M%S`

function ssh_Authen_keyfile_bak()
{
	#backup publickey file----------- 	

	dyDEBUG echo $keyfilebak_identify
	keyfiles_bak_name=authorized_keys_$keyfilebak_identify
	if [ -e ~/.ssh/authorized_keys ];then
		dyDEBUG echo "bak1"
		mv ~/.ssh/authorized_keys ~/.ssh/$keyfiles_bak_name
	fi

	if [ -e ~/.ssh/id_rsa ];then
		dyDEBUG echo "bak2"
		mv ~/.ssh/id_rsa ~/.ssh/id_rsa_$keyfilebak_identify
	fi
	if [ -e ~/.ssh/id_rsa.pub ];then
		dyDEBUG	echo "bak3"
		mv ~/.ssh/id_rsa.pub ~/.ssh/id_rsa.pub_$keyfilebak_identify
	fi

	dyDEBUG echo "backup files ok"
	touch ~/.ssh/authorized_keys
	chmod 600 ~/.ssh/authorized_keys	
}

function ssh_Authen_keyfile_restore()
{
	#restore key files---------
	cp ~/.ssh/authorized_keys_$keyfilebak_identify ~/.ssh/authorized_keys
	cp ~/.ssh/id_rsa_$keyfilebak_identify ~/.ssh/id_rsa
	cp ~/.ssh/id_rsa.pub_$keyfilebak_identify ~/.ssh/id_rsa.pub

	rm ~/.ssh/authorized_keys_*
	rm ~/.ssh/id_rsa_*
	rm ~/.ssh/id_rsa.pub_*

}

	#spawn ssh pangwz@localhost
function ssh_Authen_method_publickey()
{
	echo -e "\033[0;33m--------publickey Authen test begin --------\033[0m"
	#gen key pair
expect << eof
	spawn ssh-keygen -C "publickey_authen_test@inspur"
	expect "ssh/id_rsa): " { send "\n" }
	expect "no passphrase): "  { send "\n" }
	expect "passphrase again: " { send "\n" }

	expect eof
eof
	
	cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

	#login-----
expect << eof
	spawn ssh $ssh_Authen_user@localhost
	expect { 
		"*(yes/no)?" { send "yes\n";exp_continue; }
		"*password:" { send_user "key_error\n";exit 1 }
		"Permission denied*" { send_user "key_error\n";exit 1 }
		"*$ " { send_user "login ok\n";exit 0 }
	}
	expect timeout {exit 1}
	
	interact
eof
	keyresult=$?	
	dyDEBUG	echo "publickey last result $keyresult"
	if [ $keyresult -eq 0 ];then
		ssh_Authen_publickey_result=ok	
	else
		ssh_Authen_publickey_result=ng
	fi

	echo -e "\033[0;33m--------publickey Authen test end --------\033[0m"
}

function ssh_Authen_result
{
	echo "-----------------------"
	echo -e "password method test: \033[0;31m$ssh_Authen_password_result \033[0m"
	echo -e "publickey method test: \033[0;31m $ssh_Authen_publickey_result \033[0m"
	echo "-----------------------"
}

#--------main start---------------

ssh_Authen_user=$USER
ssh_Authen_user_pwd=$USER
ssh_Authen_method=
expect_func_test
ssh_Authen_user_decide
ssh_Authen_method_choice

#bakup key before password,because key authen has hight priority
ssh_Authen_keyfile_bak

if [ $ssh_Authen_method -eq 1 ];then
	ssh_Authen_method_password
elif [ $ssh_Authen_method -eq 2 ];then
	ssh_Authen_method_publickey
elif [ $ssh_Authen_method -eq 3 ];then
	ssh_Authen_method_password
	ssh_Authen_method_publickey
else
	echo "method choice error,do nothing"	
fi
#restore keyfiles
ssh_Authen_keyfile_restore

ssh_Authen_result
