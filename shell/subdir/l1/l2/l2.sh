#!/bin/bash
source ./lib.sh

function main(){
    echo "exec:$0 ----------- curdir:$PWD"
    echo "program:$program ----localdir:$localdir"
    funcShare "from $program"
}

main
