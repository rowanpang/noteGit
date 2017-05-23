#!/bin/bash

sed -n '/aaa/,/bbb/p' addr-1.txt
echo "-------------addr-2.txt--------------"
sed -n '/aaa/,/bbb/p' addr-2.txt
