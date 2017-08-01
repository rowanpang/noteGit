#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii
import hexdump
import fatMeta

diskblock='/dev/sdb1'
diskblock='/dev/sdc1'
# diskblock='./vbr-fat16.bin'
fvbr = fatMeta.fatVBR(diskblock)

fvbr.parser_fields()

# hexdump.strHexdump(fvbr.get_FAT())
hexdump.strHexdump(fvbr.get_rootdir_content())
