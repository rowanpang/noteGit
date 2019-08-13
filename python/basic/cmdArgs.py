#!/usr/bin/python
import sys, getopt

opts, args = getopt.getopt(sys.argv[1:], "hi:o:")

input_file=""
output_file=""

for op, value in opts:
    if op == "-i":
        input_file = value
        print '-i'
    elif op == "-o":
        output_file = value
        print '-o'
    elif op == "-h":
        print 'help---'
        # usage()
        sys.exit()

if args:
    print args

