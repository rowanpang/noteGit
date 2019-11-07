#!/bin/bash

verbose="3"
function pr_debug(){
    fmt=$1 && shift
    [ $verbose -ge 7 ] && printf "$fmt\n" "$@"
}

function pr_info(){
    fmt=$1 && shift
    [ $verbose -ge 6 ] && printf "$fmt\n" "$@"
}

function pr_notice(){
    fmt=$1 && shift
    [ $verbose -ge 5 ] && printf "$fmt\n" "$@"
}

function pr_warn(){
    #33m,yellow
    fmt=$1 && shift
    [ $verbose -ge 4 ] && printf "\033[1;33m""WARNING! $fmt\n""\033[0m" "$@"
}

function pr_err(){
    #31m,red
    fmt=$1 && shift
    [ $verbose -ge 3 ] && printf "\033[1;31m""ERROR! $fmt,Exit -1!\n""\033[0m" "$@"
    exit -1
}

function pr_hint(){
    #31m,red
    echo -e "\033[1;31m" "$@" "\033[0m"
}

function pr_devErr(){
    [ $verbose -ge 1 ] && echo "$@" >& 2
}

function testMain(){
    verbose=7
    echo "----start"
    pr_debug	"this is debug"
    pr_info	"this is info"
    pr_notice	"this is notice"
    pr_warn	"this is warn: %s,%3d\n" "warning" "10"
    pr_err	"this is err: %s" "kkkkk"

    echo "----end"
}

[ X`basename $0` == Xdebug.sh ] && testMain
