#!/bin/sh
wget 1212.ip138.com/ic.asp -qO - | iconv -f gb2312 -t utf8 | sed -n '6 s/<body.*\[\(.*\)\].*/\1\n/p'
#wget 1212.ip138.com/ic.asp -qO - | iconv -f gb2312 -t utf8 | sed -n '6p'

:<<EOF
$ curl cip.cc                                                                                                                                                                          (master↑1|✚1…2)
    IP	: 58.56.96.30
    地址	: 中国  山东  济南
    运营商	: 电信

    数据二	: 山东省济南市 | 电信

    数据三	: 中国山东省济南市 | 电信

    URL	: http://www.cip.cc/58.56.96.30
$ curl myip.ipip.net                                                               (master↑1|✚2…2)
    当前 IP：58.56.96.30  来自于：中国 山东 济南  电信
EOF
