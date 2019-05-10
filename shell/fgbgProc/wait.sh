#!/bin/bash

function sleep5(){
    sleep 5
}

function sleep10(){
    sleep 10
}

function now(){
    echo `date +%Y%m%d-%H%M%S`
}

function waitn() {
    echo "waitn start:$(now)"

    sleep5 &
    pid1=$!
    sleep10 &
    pid2=$!
    echo "waiting $pid1,$pid2---"
    wait -n $pid1 $pid2			#pid1退出,wait就返回
    echo "wait $pid1,$pid2: $?"
    wait -n $pid1 $pid2			#虽然pid1 not exist,但pid2 exist,所以可以正常wait
    echo "wait2 $pid1,$pid2: $?"

    echo "sleep 10"
    sleep 10
    wait -n $pid2			#此时pid2已经退出,wait失败, 返回127
    echo "wait $pid2: $?"

    echo "end:$(now)"
}

function waitjust() {
    echo "waitjust start:$(now)"

    sleep5 &
    pid1=$!
    sleep10 &
    pid2=$!
    echo "waiting $pid1,$pid2---"
    wait				#pid1,pid2都退出后才返回.
    echo "wait $?"			#alway return 0;

    echo "end:$(now)"
}

waitn
echo
waitjust
