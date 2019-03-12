#!/bin/sh

#./fir.sh while loop damage
echo "print from sec.sh arg0 is: $0"

function sec_print()
{
	echo "sec_print"
	sleep 10
	echo "sec_print wakeup"
}

function sec_show()
{
	echo "sec_show"
}


if [ "$1" = "sec_print" ]; then
	sec_print
fi

if test "$1" = sec_show ; then
	sec_show
fi

echo "in sec_sh direct call sec_print"
sec_print
