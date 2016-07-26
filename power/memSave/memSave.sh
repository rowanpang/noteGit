#!/bin/bash 
function do_debug(){
	env
	/home/pangwz/rpmbuild/BUILD/i3-4.0.2/i3-msg/i3-msg -v
	sleep 130
}

function do_work(){
	i3-msg 'workspace 10;exec gnome-terminal --profile=ptest --full-screen -e "cmatrix -B ";exec xtrlock-pam -p system-auth -b none' > /dev/null
	echo 'mem' > /sys/power/state
}

#---Main

debug=0

if [ $debug -eq 1 ];then
	#echo "debug----"
	do_debug
else
	#echo "work----"
	do_work
fi
