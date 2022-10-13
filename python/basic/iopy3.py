#!/usr/bin/python3
import sys
import time
import datetime
'''
    stdout
        When interactive, the stdout stream is line-buffered.
        Otherwise, it is block-buffered like regular text files.
    stderr
        The stderr stream is line-buffered in both cases.
        Changed in version 3.9: Non-interactive stderr is now line-buffered instead of fully buffered.

    global unbuffer
        1),assing the -u command-line option
        2),setting the PYTHONUNBUFFERED environment variable.

    ref
        https://docs.python.org/3/library/sys.html#sys.stdout
'''

#default non buffer IO
print('')
print('Now:',datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'));
print("--------- flush true----",flush=True);
print("--------- flush none----");
print("--------- flush false---",flush=False);

#print("--------- flush true----",flush=True);      #这里将flush output,而不单单是这条log.


time.sleep(5)
