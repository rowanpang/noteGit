#!/bin/bash
source ./lib.sh libinit
program=$0

function main(){
    echo "exec:$program ----------- curdir:$PWD"
    echo "var $varValuedbylibInit"
    echo "libinited $libinited"
    echo
    for cute in `find ./ -mindepth 2 -name "*.sh"`;do
	echo $cute
	$cute
	echo 
    done
}

main
