#!/bin/bash

target=hosts-$(date +%Y%m%d)
#echo $target
date +%M%S
wget --no-check-certificate https://raw.githubusercontent.com/racaljk/hosts/master/hosts -O $target
ln -sf $target hosts

function isExist() {
	#echo "test $1"
	[ $1 == "hosts" ] || [ $1 == $target ] || return 1
}

function pureCommit {
	for changed in `git status -s | awk {'print $2'}`;do
		isExist $changed
		if [  $? -eq 0 ];then
			#echo "exist"
			continue
		else
			#echo "not exist"
			return 1
		fi
	done
}

pureCommit
ret=$?
git add $target
git add hosts
if [ $ret -eq 0 ];then
	echo "can commit"
	git commit
else
	echo "can not auto commit,manual do it"
fi
		
