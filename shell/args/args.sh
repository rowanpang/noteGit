#!/bin/bash

echo "-var replace--"
echo ${1+"$@"}
echo ${1-"$@"}

echo "-while --"
while [ $# -gt 0 ]; do
	echo '$# = '$#
	shift 3
done 

echo "-for dir files--"
for file in * ;do
	echo $file;
done
