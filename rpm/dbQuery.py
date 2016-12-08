#!/usr/bin/python

import rpm

ts = rpm.TransactionSet()
# mi = ts.dbMatch()
mi = ts.dbMatch('name','rpm')
for h in mi:
        print "%s-%s-%s" %(h['name'],h['version'],h['release'])
        # for f in h['FILENAMES']:
            # print f
        print "%s" %h[rpm.RPMTAG_NAME]
