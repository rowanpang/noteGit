#!/bin/bash

trap 'onCtrlC' SIGINT
function onCtrlC () {
    echo "Ctrl+C is captured by $0"
    #need explicit or it will continue
    exit 1
}

while true; do
    echo 'I am working!'
    yes >/dev/null 2>&1 &       #父进程退出不会kill 子进程
    sleep 1
done
