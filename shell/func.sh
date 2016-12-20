#!/bin/sh

function f1(){
	echo "f1"
	#return "fun1"
	echo "debug in fun1" > /dev/null
}

function f2(){
	echo "f2"
	local str=$(f1)
	echo $str
	#return "fun2"
}

function lsudo(){
	#local cmd="$@";sudo $cmd   not use this type,error! for $@ conatin ' '"
	sudo "$@"
}

lsudo sed 's#auth\s\+sufficient\s\+pam_unix.so\s\+#& nodelay #' /etc/pam.d/system-auth
