#!/usr/bin/python
import collections


CR = '\r'
LF = '\n'
CRLF = '\r\n'

source = '''a\r\n
b\r
c
d
e'''

def find_newline(source):
    """Return type of newline used in source.

    Input is a list of lines.

    """
    assert not isinstance(source, unicode)

    counter = collections.defaultdict(int)
    for line in source:
        if line.endswith(CRLF):
            counter[CRLF] += 1
        elif line.endswith(CR):
            counter[CR] += 1
        elif line.endswith(LF):
            counter[LF] += 1

    print counter.items()
    print counter.get(CR)
    print counter.keys()
    print type(counter)
    print type(counter.items())
    print '   sort x.items: %s' %(sorted(counter.items()))
    print '         sort x: %s' %(sorted(counter))
    print 'sort x with key: %s' %(sorted(counter, key=counter.get, reverse=True))

    for c in counter:
        print c
        print type(c)

    return (sorted(counter, key=counter.get, reverse=True) or [LF])[0]

print find_newline(source)

d={'a':'bbbb','b':'aaaa'}
for a in d:
    print a
print sorted(d)
