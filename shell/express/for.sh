#!/bin/bash

function fun(){
    for i in a b c;do
	if [ $i == 'a' ];then
	    continue
	fi
	echo $i
    done
}

function _fun(){
    fun zz bb

    for char in {1..4};do
	echo $char
    done

    for (( i = 0; i < 10; i++ )); do
	echo $i
    done

    for i in `seq 1 1024`; do
	echo "echo $i > /tmp/$i &" | bash;
    done
}

_fun
