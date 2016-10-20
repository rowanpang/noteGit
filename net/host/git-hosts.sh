#!/bin/sh

if ! [ -f /tmp/rowan/github/net/host/hosts ];then
	git clone git@github.com:rowanpang/test.git /tmp/rowan/github
fi

cp /tmp/rowan/github/net/host/hosts /etc/hosts
