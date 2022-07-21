#!/bin/bash
#"中":Unicode 0x4E2D
#"文":Unicode 0x6587

echo "中文" > file_utf.txt
iconv -f utf8 -t utf-16 -o file_ucs.txt ./file_utf.txt

out=$(xxd -g 1 ./file_ucs.txt | awk '{printf $5$4 $7$6 $8$9"\n"}')
#echo $out
if [ $out = "4e2d65870a00" ];then
	echo "ok"
else
	echo "ng"
fi
