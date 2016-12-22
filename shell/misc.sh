#!/bin/sh

function lsudo(){
	sudo "$@"
}

function now(){
	echo `date +%Y%m%d-%H%M%S`
}
lsudo sed 's#auth\s\+sufficient\s\+pam_unix.so\s\+#& nodelay #' /etc/pam.d/system-auth

echo "$(now)"
sleep 1
echo "$(now)"
