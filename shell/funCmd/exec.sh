#!/bin/bash 

echo "exec testing"
echo $$
exec /usr/bin/vim a.txt
echo "exec finished"		#will not show this msg
