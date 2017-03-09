#!/bin/bash
let size=1024*5; dd if=/dev/urandom bs=$size count=1 2>/dev/null | base64 > random.txt
./kpr random.txt
