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
