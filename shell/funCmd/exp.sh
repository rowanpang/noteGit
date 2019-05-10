#!/bin/sh

function f1(){
    echo "f1"
    #return "fun1"
    echo "debug in fun1" > /dev/null
}

function f2(){
    echo "f2"
    local str=$(f1)
    echo $str
    #return "fun2"
}

function lsudo(){
    #local cmd="$@";sudo $cmd   not use this type,error! for $@ conatin ' '"
    sudo "$@"
}

dir="adf" f2
echo $dir
echo
dir="adf";f2
echo $dir

function test_func() {
    echo "Current $FUNCNAME, \$FUNCNAME => (${FUNCNAME[@]})"
    another_func
    echo "Current $FUNCNAME, \$FUNCNAME => (${FUNCNAME[@]})"
}

function another_func() {
    echo "Current $FUNCNAME, \$FUNCNAME => (${FUNCNAME[@]})"
}

echo "Out of function, \$FUNCNAME => (${FUNCNAME[@]})"
test_func
echo "Out of function, \$FUNCNAME => (${FUNCNAME[@]})"

function funReturn() {
    echo "hello"
}

str=`funReturn`
echo "return var:$?"

