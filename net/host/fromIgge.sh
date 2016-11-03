#!/bin/bash

target=hosts-$(date +%Y%m%d)
#echo $target
wget $1 | tee $target
ln -sf $target hosts


