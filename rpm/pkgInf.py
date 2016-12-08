#!/usr/bin/python
# Lists information on installed package listed on command line.
# Usage:
# python rpminfo.py package_name
import os
import sys
import rpm
def printEntry(header, label, format, extra):
    value = header.sprintf(format).strip()
    print "%-20s: %s %s" % (label, value, extra)

def printHeader(h):
    if h[rpm.RPMTAG_SOURCEPACKAGE]:
        extra = " source package"
    else:
        extra = " binary package"
    printEntry(h, 'Package', "%{NAME}-%{VERSION}-%{RELEASE}", extra)
    printEntry(h, 'Group', "%{GROUP}", '')
    printEntry(h, 'Summary', "%{Summary}", '')
    printEntry(h, 'Arch-OS-Platform', "%{ARCH}-%{OS}-%{PLATFORM}", '')
    printEntry(h, 'Vendor', "%{Vendor}", '')
    printEntry(h, 'URL', "%{URL}", '')
    printEntry(h, 'Size', "%{Size}", '')
    printEntry(h, 'Installed on', "%{INSTALLTID:date}", '')
    printEntry(h, 'payloadFormat', "%{PAYLOADFORMAT}", '')
    printEntry(h, 'payloadCompressor', "%{payloadcompressor}", '')
    print
    print h['description']


ts = rpm.TransactionSet()
fdno = os.open(sys.argv[1],os.O_RDONLY)
hdr = ts.hdrFromFdno(fdno)
printHeader(hdr)
os.close(fdno)
# mi = ts.dbMatch( 'name', sys.argv[1] )
# for h in mi:
    # printHeader(h)
