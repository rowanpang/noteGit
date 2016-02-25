#!/bin/sh

echo "print from $0"

function thr_print()
{
	echo "hello world from $0"
}

echo "exec sec.sh,args:'sec_print' "

#will fork a new process and run ./sec.sh

./sec.sh sec_print

#not include,so sec_show can not found
sec_show
