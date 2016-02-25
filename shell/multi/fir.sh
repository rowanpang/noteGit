#!/bin/sh

echo "source sec.sh"
. ./sec.sh

echo "print from $0"

function fir_print()
{
	echo "hello world from $0"
}

echo "call sec.sh func 'sec_print'"
./sec.sh sec_print

sec_show

sleep 10
