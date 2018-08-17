#!/bin/bash

for i in `seq 1 5`;do
    ar[$i]=$i
done

echo ${ar[@]}
