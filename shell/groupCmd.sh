#!/bin/bash
#ref https://unix.stackexchange.com/questions/88850/precedence-of-the-shell-logical-operators

false && echo "true" || echo "false"	    #echo 'false‘
true && echo "true" || echo "false"	    #echo 'true'

echo "test1" || { echo "sadfddd" && exit -1; }
echo "finish"
