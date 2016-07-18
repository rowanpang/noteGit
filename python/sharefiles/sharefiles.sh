#/bin/bash
ip a s | grep inet\ 

port=8000
if [ $# -ge 1 ];then
	port=$1
	#echo "Use port: $port"
fi

python -m SimpleHTTPServer $port


