#!/bin/bash

function prepareGroupxml(){
    org=$(python << "_EOF"
import xml.etree.ElementTree as ET
import hashlib
import binascii
import sys

tree = ET.parse('./repodata/repomd.xml')
root = tree.getroot()
grp = root.find('./{http://linux.duke.edu/metadata/repo}data[@type="group"]')
for c in grp.getchildren():
    if c.tag.endswith('location'):
	fPath = c.attrib['href']
	# print fPath
    if c.tag.endswith("checksum"):
	hashT = c.attrib["type"]
	hashV = c.text
	# print hashT
	# print hashV

hobj = hashlib.new(hashT)
hobj.update(open(fPath).read())

hashCal = binascii.hexlify(hobj.digest())

if hashV == hashCal:
    #os.system('cp %s groupXml.xml' %(fPath))
    print fPath
else:
    print 'org groupxml fail'
    sys.exit(-1)
_EOF
)
    ret=$?
    if [ $ret -eq 0 ];then
	echo $org
    fi
    return $ret
}

echo $(prepareGroupxml)
