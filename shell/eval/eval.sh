#!/bin/sh

. ./functions.sh
DATA="$(cat input)"
eval "$DATA"

newline=$'\n'    
	#ref http://stackoverflow.com/questions/17821277/how-to-separate-multiple-commands-passed-to-eval-in-bash

eval "config 'system'  'rowanTest' $newline config 'system'  'pangwztest'"
	#eval 首先展开参数，然后将参数作为命令输入，执行.
    #换行符需要特殊处理.

export | grep CONFIG_

unset DATA
