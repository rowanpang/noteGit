#!/bin/bash

tempdirs="-e /etc -e /selinux"
tmpdirs=${tempdirs//-e/}
for p in ${tempdirs}
do
	echo ${p}
done

spaceString="     asdf"
echo $spaceString

str="abcd"
echo ${str:0:10}

fileName="module.c.conf"
echo ${fileName/./_}
echo ${fileName//./_}

testOut="asdfasdf   
adfkajdfla"

testOut2="$(echo "$testOut" | sed ':a;N;$!ba;s#\n#\\n#')"
echo "$testOut"
echo -e $testOut
echo $testOut2
