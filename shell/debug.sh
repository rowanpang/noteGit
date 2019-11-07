#!/bin/bash

verbose="3"
function pr_debug(){
    [ $verbose -ge 7 ] && echo "$@"
}

function pr_info(){
    [ $verbose -ge 6 ] && echo "$@"
}

function pr_notice(){
    [ $verbose -ge 5 ] && echo "$@"
}

function pr_warn(){
    #33m,yellow
    [ $verbose -ge 4 ] && echo -e "\033[1;33m"WARNING! "$@" "\033[0m"
}

function pr_err(){
    #31m,red
    [ $verbose -ge 3 ] &&echo -e "\033[1;31m"ERROR! "$@",exit -1 "\033[0m" >& 2
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
    pr_debug	"this is debug"
    pr_info	"this is info"
    pr_notice	"this is notice"
    pr_warn	"this is warn"
    pr_err	"this is err"
}

[ X`basename $0` == Xdebug.sh ] && testMain
