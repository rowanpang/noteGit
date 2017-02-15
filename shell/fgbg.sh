#!/bin/sh

stdInDev=`readlink /proc/self/fd/0`	#此时self代表了readlink 进程.
stdOutDev=`readlink /proc/self/fd/1`
stdErrDev=`readlink /proc/self/fd/2`
echo $stdInDev > log
echo $stdOutDev >> log
echo $stdErrDev >> log

echo >> log
echo >> log

echo $$ >> log
stdInDev=`readlink /proc/$$/fd/0`
stdOutDev=`readlink /proc/$$/fd/1`
stdErrDev=`readlink /proc/$$/fd/2`
echo $stdInDev >> log
echo $stdOutDev >> log
echo $stdErrDev >> log
#sleep 30

read a
echo $a >> log

