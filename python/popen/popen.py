#!/usr/bin/python
import os
import subprocess
if __name__ == '__main__':
	p=subprocess.Popen("./a.out",stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
	p.stdin.write('3\n')
	p.stdin.write('3\n')
	print p.stdout.read()
