#!/bin/bash
. ./lib.sh

program=$0
function main(){
    echo "exec:$program ----------- curdir:$PWD"
    funcShare
}

main
