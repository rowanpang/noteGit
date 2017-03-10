#!/bin/bash
. ./lib.sh

program=$0
function main(){
    echo "exec:$program ----------- curdir:$PWD"
    #sourced but not do libinit so,not valued
    echo "var $varValuedbylibInit"
    echo "libinited $libinited"

    funcShare
}

main
