#!/bin/bash

#ref https://stackoverflow.com/questions/1251999/how-can-i-replace-a-newline-n-using-sed
testOut="asdfasdf   
adfkajdfla"

testOut2="$(echo "$testOut" | sed ':a;N;$!ba;s#\n#\\n#')"                               
echo "$testOut"
echo -e $testOut
echo $testOut2

