#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii
import hexdump
import fatMeta

diskblock='/dev/sdb1'
diskblock='/dev/sdc1'
# diskblock='/dev/sdc2'
# diskblock='./vbr-fat16.bin'
fvbr = fatMeta.fatVBR(diskblock)

fvbr.parser_fields()

# hexdump.strHexdump(fvbr.get_FAT())
# b= fvbr.read_cluster(2)
# b,i = fvbr.read_cluster_chain(2,0)
# hexdump.strHexdump(b)

# hexdump.strHexdump(fvbr.get_rootdir_content())
fvbr.list_root()

