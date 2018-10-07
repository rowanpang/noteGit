#!/bin/bash

echo "in read.sh"
echo "arg1: $1"
echo "starting read arg1,2,3"
read arg1 arg2 arg3
printf "args: %s %s %s\n" $arg1 $arg2 $arg3

stty -echo
echo "echo off ,reading pwd"
read pwd
printf "pwd is:%s \n" $pwd
stty echo

echo "echo on,read -s -p"
read -s -p "Password: " pwd
printf "pwd is:%s \n" $pwd

unset pwd
prompt="Password: "
while IFS= ;read -p "$prompt" -r -s -n 1 pchar;do
    if [[ "$pchar" == $'\0' ]];then
	break;
    fi

    prompt="*"
    pwd+="$pchar"
done
echo

printf "pwd is:%s \n" "$pwd"
