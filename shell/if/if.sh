#!/bin/bash

if ! grep -q "self" ./test.txt;then
	echo "grep ok";
else
	echo "grep ng"
fi
