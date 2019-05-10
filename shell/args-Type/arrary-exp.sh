#!/bin/bash

b=(
    aaaa
    bbbb
    cccc
    dddd
    eeee
)

for i in `seq 1 5`;do
    ar[$i]=$i
done

echo ${ar[@]}
for i in ${b[@]};do
    echo $i
done
