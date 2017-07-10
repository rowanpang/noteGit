#!/usr/bin/python
import sys
import os

i = 0
nf = open("/tmp/asdfadf","w")
for l in open("/etc/ssl/certs/ca-bundle.crt","r"):
    if l.startswith('-----BEGIN CERTIFICATE-----'):
        # print "-------- %s-------" %(i)
        fname = './py-indivi-%s' %(i)
        nf = open(fname,"w")
        i += 1

    nf.write(l)

    if l.startswith('-----END CERTIFICATE-----'):
       nf.close()

print('total num:%s' %(i))

os.system('csplit -z -s -f sh-indivi- /etc/ssl/certs/ca-bundle.crt "/-----END CERTIFICATE-----/1" "{*}"')

os.system('''awk '
        split_after == 1 {n++;split_after=0}
        /-----END CERTIFICATE-----/ {split_after=1}
        {print > "awk-cert" n ".pem"}' < /etc/ssl/certs/ca-bundle.crt'''
    )

