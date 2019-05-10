#!/bin/bash

echo "in run.sh exec testing arg0: $0"
exec ./hello.sh
echo "in run.sh exec finished"		#will not show this msg
