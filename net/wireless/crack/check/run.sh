#!/bin/bash

[ $UID != 0 ] && echo -e "\033[0;31m------USE ROOT---------\033[0;0m" && exit

rm -rf radioCheck-*.csv
rm -rf outSorted.csv

airmon-ng start wlp2s0
timeout --foreground 20s airodump-ng -w radioCheck --output-format csv wlp2s0mon

if [ -s radioCheck* ];then
    ./sort.py radioCheck*  | tee outSorted.csv
fi
