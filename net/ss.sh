#!/bin/bash
case "$1" in
    start)
    sudo sslocal -s 121.42.41.165 -b 192.168.1.100 -p 443 -l 8080 -k QZPMqzpm -d start
    ;;
    stop)
    sudo sslocal -s 121.42.41.165 -b 192.168.1.100 -p 443 -l 8080 -k QZPMqzpm -d stop
    ;;
    *)
    sudo sslocal -s 121.42.41.165 -b 192.168.1.100 -p 443 -l 8080 -k QZPMqzpm -d restart
    ;;
esac
