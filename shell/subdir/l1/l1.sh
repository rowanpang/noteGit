#!/bin/bash
. ./lib.sh

function main(){
    echo "exec:$0 ----------- curdir:$PWD"
    echo "program:$program ----localdir:$localdir"
    #sourced but not do libinit so,not valued
    echo "var $varValuedbylibInit"
    echo "libinited $libinited"


    funcShare
}

main
