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

function fun1(){
    arg1=$1
    arg2=$2
    args="$@"

    echo $arg1
    echo $arg2
    echo $args
}

fun1 "1 2 3" "4"
