#!/bin/bash
source ./lib.sh

program=$0
function main(){
    echo "exec:$program ----------- curdir:$PWD"
    funcShare "from $program"
}

main
