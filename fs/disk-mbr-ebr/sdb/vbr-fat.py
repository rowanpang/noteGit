#!/usr/bin/python
# -*- coding: utf-8 -*-

import binascii
import hexdump

diskblock='/dev/sdb'

vbr = open(diskblock,'r').read(512)
hexdump.strHexdump(vbr)

