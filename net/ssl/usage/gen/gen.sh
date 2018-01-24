#!/bin/bash

openssl req -new -nodes -utf8 -sha1 -days 36500 -batch -x509 -config x509.genkey -outform PEM -out signing_key_rowan.x509 -keyout signing_key_rowan.priv
