#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii
import hexdump
import fatMeta

diskblock='/dev/sdb1'
fvbr = fatMeta.fatVBR(diskblock)

print fvbr.print_fields()
