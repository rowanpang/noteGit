#!/usr/bin/env python
#encoding: utf-8
import sys
import urllib2
from base64 import b64decode

bl_file = 'gfw_dstdom_regex'
acl_name_bl = 'gfw'
wh_file = 'cn_dstdom_regex'
acl_name_wh = 'cn'

acl_type = 'dstdom_regex'

def parse_line(line):
	if line[0] == '/' and line[-1] == '/':   #regex line
		return line[1:-1]
	
	line = line.replace('*','.+')
	line = line.replace('(',r'\(').replace(')',r'\)')

	if line.startswith('||'):
		return '.' + line[2:]
	elif line.startswith('|https://'):
		return line[9:]
	elif line.startswith('|http://'):
		return line[8:]
	elif line.startswith('|'):
		return line[1:]
	else:
		return line

def convert(gfwlist):
	black = open(bl_file,'w')
	white = open(wh_file,'w')
	for l in gfwlist.split('\n'):
		if not l or l[0] == '!' or l[0] == '[':
			continue

		if l.startswith('@@'):
			white.write('acl ' + acl_name_wh + ' ' + acl_type + ' ' + parse_line(l[2:]) + '\n')
		else:
			black.write('acl ' + acl_name_bl + ' ' + acl_type + ' ' + parse_line(l) + '\n')

def main():
	print "main test"
	print __file__
	print urllib2.__file__
	
	if len(sys.argv) < 2:
		print "arg error"
		return

	src = open(sys.argv[1]).read()
	src = b64decode(src)
	# print src
	convert(src)


if __name__ == '__main__':
	main()

