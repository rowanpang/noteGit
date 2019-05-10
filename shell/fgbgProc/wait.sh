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

echo "start:$(now)"

sleep5 &
sleep10 &

echo "waiting---"
wait

echo "end:$(now)"
