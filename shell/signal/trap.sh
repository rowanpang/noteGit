#!/bin/bash

trap 'onCtrlC' INT
function onCtrlC () {
    echo 'Ctrl+C is captured'
    #need explicit or it will continue
    exit 1
}

while true; do
    echo 'I am working!'
    sleep 1
done
