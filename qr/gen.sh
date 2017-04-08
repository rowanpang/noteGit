#!/bin/bash

for f in *.txt;do 
    qrencode -t png -o png-${f%.txt}.png < $f
done
