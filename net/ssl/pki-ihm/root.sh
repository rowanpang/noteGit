#!/bin/bash

#openssl req -new -nodes -utf8 -sha1 -days 36500 -batch -x509 -outform PEM -out ca.x509 -keyout key.priv
cfg="root.cfg"

prefix=$(grep output_password $cfg | awk 'BEGIN{FS = "="};{print $2}')
cert="$prefix.cer"

if ! [ -f $cert ] || [ $# -gt 1 ];then
    openssl req -new -x509 -days 3650 -config $cfg -outform PEM -out $cert
fi
openssl x509 -in $cert -inform PEM -noout -text
