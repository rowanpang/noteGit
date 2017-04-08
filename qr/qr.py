#!/usr/bin/python

import sys
import qrtools

qr = qrtools.QR()
qr.decode("png-vCard.png")
print qr.data
