#!/bin/bash

echo "filename: $0"
echo "args NUm: $#"

echo "-var replace--"
echo ${1+"$@"}
echo ${1-"$@"}

echo "-while --"
while [ $# -gt 0 ]; do
    echo "arg is = $#"
    shift 
done 

echo "-for dir files--"
for file in * ;do
    echo $file;
done
