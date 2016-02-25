#!/bin/bash

tempdirs="-e /etc -e /selinux"
tmpdirs=${tempdirs//-e/}
for p in ${tempdirs}
do
	echo ${p}
done
