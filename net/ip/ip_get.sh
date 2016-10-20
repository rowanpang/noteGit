#!/bin/sh
wget 1212.ip138.com/ic.asp -qO - | iconv -f gb2312 -t utf8 | sed -n '6 s/<body.*\[\(.*\)\].*/\1\n/p'
#wget 1212.ip138.com/ic.asp -qO - | iconv -f gb2312 -t utf8 | sed -n '6p'
