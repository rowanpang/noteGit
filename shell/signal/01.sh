#!/bin/bash

trap 'onCtrlC' SIGINT
function onCtrlC () {
    echo "Ctrl+C is captured by $0"
    #need explicit or it will continue
    exit 1
}

./02.sh

while true; do
    echo 'I am working!'
    sleep 1
done
