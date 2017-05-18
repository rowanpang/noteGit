#!/bin/bash
while [ "aa" ];do
    echo -n "please input:"
    read aNum
    case $aNum in
	    1)  
		echo 'You select 1'
		;;
	    [2-4])
		echo "match [2-4],$aNum"
		;;
	    5|6)  
		echo 'You select 5 or 6'
		;;
	    [:digit:])				#ng
		echo "match [:digit:],$aNum"
		;;
	    [8-12])				#ng
		echo "match [8-12],$aNum"
		;;
	    [7-9][7-9])				
		echo "match [7-9][7-9],$aNum"
		;;
	    ?[7-9])				#ng
		echo "match ?[7-9],$aNum"
		;;
	    [abcd])				#match single a,b,c or d
		echo "match [abcd],$aNum"
		;;
	    [abcd]*)				#match string start with a,b,c or d
		echo "match [abcd]*,$aNum"
		;;

	    *)  
		echo 'no match'
		;;
    esac
done
