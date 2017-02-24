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
}

_fun
